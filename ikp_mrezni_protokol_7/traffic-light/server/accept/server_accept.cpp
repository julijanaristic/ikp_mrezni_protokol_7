#include "server_accept.h"
#include "../thread_pool/thread_pool.h"

#include "../../common/protocol/message.h"
#include "../../common/protocol/message_codec.h"

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <atomic>

static std::atomic<int> g_nextClientId{1};

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
        info.clientId = g_nextClientId++;
        info.active = true;

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.insert(info.clientId, info);
        }

        Protocol::Message hello(info.clientId, Protocol::MessageType::ACK, "ASSIGNED_ID");

        std::string raw = Protocol::serialize(hello);
        send(clientSocket, raw.c_str(), raw.size(), 0);

        threadPool.addClient(clientSocket);
    }
}

void AcceptThread::removeClient(int clientId) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    ClientInfo* info = clients.find(clientId);
    if(info){
        close(info->socketFd);
        clients.remove(clientId);
    }
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
