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

        //slanje komande
        const char* command = "COMMAND GREEN\n";
        send(clientFd, command, strlen(command), 0);

        //citanje odgovora
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(clientFd, buffer, sizeof(buffer), 0);
        if(bytes <= 0) {
            std::cout << "[THREAD] Client disconnected\n";
            close(clientFd);
            continue;
        }

        //obrada odgovora
        //std::cout << "[THREAD] Received: " << buffer << std::endl;

        if(strncmp(buffer, "ACK", 3) == 0){
            std::cout << "[SERVER] OK from client\n";
        }
        else if(strncmp(buffer, "ERROR", 5) == 0){
            std::cout << "[SERVER] ERROR from client: " << buffer << std::endl;
        }
        else if(strncmp(buffer, "CLIENT_EXIT", 11) == 0){
            std::cout << "[SERVER] Client requested exit\n";
            close(clientFd);
        }
        else {
            std::cout << "[SERVER] Unknown response: " << buffer << std::endl;
        }

        const char* ack = "ACK\n";
        send(clientFd, ack, strlen(ack), 0);
    }
}