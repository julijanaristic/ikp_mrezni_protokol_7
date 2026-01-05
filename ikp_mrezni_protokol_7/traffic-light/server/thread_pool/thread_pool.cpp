#include "thread_pool.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>

ThreadPool::ThreadPool(int numThreads): head(nullptr), tail(nullptr), running(true), threadCount(numThreads) {
    workers = new std::thread[threadCount];
    for (int i=0; i<threadCount; i++)
        workers[i] = std::thread(&ThreadPool::workerLoop, this);
}

ThreadPool::~ThreadPool() {
    running = false;
    cv.notify_all();

    for (int i=0; i<threadCount; i++){
        if (workers[i].joinable())
            workers[i].join();
    }

    delete[] workers;
}

void ThreadPool::addClient(int socketFd) {
    ClientTask* task = new ClientTask{socketFd, nullptr};

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!tail){
            head = tail = task;
        }
        else {
            tail->next = task;
            tail = task;
        }
    }

    cv.notify_one();
}

void ThreadPool::workerLoop() {
    while (running) {
        ClientTask* task = nullptr;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [&]() { return head != nullptr || !running; });

            if (!running) 
                return;
            
            task = head;
            head = head->next;
            if (!head)
                tail = nullptr;
        }

        int clientFd = task->socketFd;
        delete task;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(clientFd, buffer, sizeof(buffer), 0);
        if(bytes <= 0) {
            close(clientFd);
            continue;
        }

        std::cout << "[THREAD] Received: " << buffer << std::endl;

        const char* ack = "ACK\n";
        send(clientFd, ack, strlen(ack), 0);
    }
}