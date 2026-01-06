#include "server_accept.h"
#include "../thread_pool/thread_pool.h"

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

AcceptThread::AcceptThread(int serverSocket, ThreadPool& pool): serverSocket(serverSocket), threadPool(pool), running(false) {}

AcceptThread::~AcceptThread() {
    stop();
}

void AcceptThread::start() {
    running = true;
    acceptThread = std::thread(&AcceptThread::acceptLoop, this);
}

void AcceptThread::stop() {
    // obratiti paznju kasnije ako je nit blokirana, join ce cekati zauvijek

    // treba zatvoriti server socket? 
    // ili koristiti shutdown?
    running = false;

    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);

    if (acceptThread.joinable())
        acceptThread.join();
}

void AcceptThread::acceptLoop() {
    while (running) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        if (clientSocket < 0)
            continue;
        
        ClientInfo info;
        info.socketFd = clientSocket;
        info.active = true;

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.insert(clientSocket, info);
        }

        threadPool.addClient(clientSocket);
    }
}

void AcceptThread::removeClient(int socketFd) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.remove(socketFd);
    close(socketFd);
}

void shutdownClient(ClientInfo& info){
    const char* msg = "SHUTDOWN\n";
    send(info.socketFd, msg, strlen(msg), 0);
    close(info.socketFd);
    info.active = false;
}

void AcceptThread::shutdownAllClients(){
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.forEach(shutdownClient);
}