#include "server_accept.h"
#include "../thread_pool/thread_pool.h"

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <string>

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

namespace {
    const char* g_msg = nullptr;

    void sendToClient(ClientInfo& info) {
        if (!info.active) return;
        send(info.socketFd, g_msg, strlen(g_msg), 0);
    }
}

void AcceptThread::broadcast(const std::string& msg) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    g_msg = msg.c_str();
    clients.forEach(sendToClient);
    g_msg = nullptr;
}
