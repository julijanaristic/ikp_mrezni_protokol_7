#ifndef SERVER_ACCEPT_H
#define SERVER_ACCEPT_H

#include <mutex>
#include <thread>

#include "../container/client_map.h"

class ThreadPool;

class AcceptThread {
    public:
        AcceptThread(int serverSocket, ThreadPool& pool);
        ~AcceptThread();

        void start();
        void stop();

        void removeClient(int socketFd);
        void shutdownAllClients();
        void broadcast(const std::string& msg);
        
    private:
        void acceptLoop();

        int serverSocket;
        ThreadPool& threadPool;

        ClientMap clients;
        std::mutex clientsMutex;

        std::thread acceptThread;
        bool running;
};

#endif 