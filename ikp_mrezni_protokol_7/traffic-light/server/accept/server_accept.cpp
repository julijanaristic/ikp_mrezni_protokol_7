#include "server_accept.h"
// #include "thread_pool.h"

#include <sys/socket.h>
#include <unistd.h>

AcceptThread::AcceptThread(int serverSocket, ThreadPool& pool): serverSocket(serverSocket), threadPool(pool), running(false) {}

AcceptThread::~AcceptThread() {
    stop();
}

void AcceptThread::start() {
    running = true;
    acceptThread = std::thread(&AcceptThread::acceptLoop, this);
}

void AcceptThread::stop() {
    running = false;

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