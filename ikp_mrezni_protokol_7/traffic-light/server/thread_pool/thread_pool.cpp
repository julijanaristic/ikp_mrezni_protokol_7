#include "thread_pool.h"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>

#include "../../common/protocol/message.h"
#include "../../common/protocol/message_codec.h"

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
        
        while(running){           
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(clientFd, buffer, sizeof(buffer), 0);

            if(bytes <= 0) {
                std::cout << "[SERVER] Client disconnected\n";
                close(clientFd);
                break;
            }

            std::string raw(buffer);

            Protocol::Message msg;
            try {
                msg = Protocol::deserialize(raw);
            } catch (const std::exception& e) {
                std::cerr << "[SERVER] Failed to deserialize message: " << e.what() << "\n";
                continue;             
            }

            switch (msg.type) {
                case Protocol::MessageType::ACK: 
                    std::cout << "[SERVER] ACK from client " << msg.clientId << "\n";
                    break;

                case Protocol::MessageType::ERROR:
                    std::cout << "[SERVER] ERROR from client " << msg.clientId << ": " << msg.payload << "\n";
                    break;

                case Protocol::MessageType::CLIENT_EXIT:
                    std::cout << "[SERVER] Client exit: " << msg.clientId << "\n";
                    close(clientFd);
                    return;
                
                default:
                    std::cout << "[SERVER] Unknown message from client " << msg.clientId << "\n";
                    break;
            }
        }
    }
}

void ThreadPool::shutdownAll(){
    // running = false;

    // cv.notify_all(); //probuditi sve niti

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        running = false;

        while (head) {
            ClientTask* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
    }
    cv.notify_all();
}