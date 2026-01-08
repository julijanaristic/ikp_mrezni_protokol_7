#ifndef EXECUTION_THREAD_H
#define EXECUTION_THREAD_H

#include "../traffic_light/traffic_light.h"
#include "../queue/command_queue.h"
#include <thread>
#include <atomic>

class ExecutionThread {
    public:
        ExecutionThread(TrafficLight& light, CommandQueue& queue);
        ~ExecutionThread();

        void start();
        void stop();

    private:
        void run();

        TrafficLight& trafficLight;
        CommandQueue& commandQueue;

        std::thread worker;
        std::atomic<bool> running;
};

#endif