#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "../traffic_light/command.h"
#include <mutex>
#include <condition_variable>

class CommandQueue {
    public:
        CommandQueue();
        ~CommandQueue();

        void push(Protocol::Light light);
        bool pop(Protocol::Light& out);

        bool empty();

    private:
        Command* head;
        Command* tail;

        std::mutex mtx;
        std::condition_variable cv;
};

#endif