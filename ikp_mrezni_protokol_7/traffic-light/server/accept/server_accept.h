#ifndef SERVER_ACCEPT_H
#define SERVER_ACCEPT_H

#include <mutex>
#include <thread>

#include "client_map.h"

class ThreadPool;

class AcceptThread {
    public:
        AcceptThread(int serverSocket, ThreadPool& pool);
        ~AcceptThread();

        void start();
        void stop();

        void removeClient(int socketFd);

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