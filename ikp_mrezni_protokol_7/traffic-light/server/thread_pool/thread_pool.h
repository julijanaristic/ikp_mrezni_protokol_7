#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "../model/client_task.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool {
    public:
        ThreadPool(int numThreads = 4);
        ~ThreadPool();

        void addClient(int socketFd);
        void shutdownAll();
    private:
        void workerLoop();

        // red klijenata
        ClientTask* head;
        ClientTask* tail;

        // sinhronizacija
        std::mutex queueMutex;
        std::condition_variable cv;
        bool running;

        // niti
        std::thread* workers;
        int threadCount;

        //za uredan ispis na terminalu 
        static std::mutex logMutex;
};

#endif