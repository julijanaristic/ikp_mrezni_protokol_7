#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "../traffic_light/command.h"
#include "../../common/protocol/protocol.h"
#include <iostream>
#include <mutex>
#include <condition_variable>

class CommandQueue {
    public:
        CommandQueue();
        ~CommandQueue();

        void push(Protocol::Light light);
        bool pop(Protocol::Light& out);

        bool empty();
        void printQueue();
        Protocol::Light getLastOrCurrent(Protocol::Light current);

    private:
        Command* head;
        Command* tail;

        std::mutex mtx;
        std::condition_variable cv;
};

#endif