#include "execution_thread.h"
#include <iostream>

ExecutionThread::ExecutionThread(TrafficLight& light, CommandQueue& queue): trafficLight(light), commandQueue(queue), running(false) {}

ExecutionThread::~ExecutionThread() {
    stop();
}

void ExecutionThread::start() {
    running = true;
    worker = std::thread(&ExecutionThread::run, this);
}

void ExecutionThread::stop() {
    running = false;
    if(worker.joinable())
        worker.join();
}

void ExecutionThread::run() {
    while (running) {
        std::cout << "[CLIENT] Light active\n";
        trafficLight.waitCurrent();

        Protocol::Light requested;

        if (commandQueue.pop(requested)) {
            if (trafficLight.isValidTransition(trafficLight.getCurrent(), requested)) {
                std::cout << "[CLIENT] Applying command\n";
                trafficLight.set(requested);
            }
            else {
                std::cout << "[CLIENT] Invalid command -> ERROR\n";
                // kasnije se salje error serveru
                trafficLight.next();
            }
        }
        else {
            //trafficLight.next();
        }
    }
}