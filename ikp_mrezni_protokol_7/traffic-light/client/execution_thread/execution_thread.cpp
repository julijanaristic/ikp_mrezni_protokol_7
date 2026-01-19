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
        std::cout << "[CLIENT] Light active: "
                  << Protocol::lightToString(trafficLight.getCurrent())
                  << std::endl;
        
        trafficLight.waitCurrent();
        
        if(!running) break;

        Protocol::Light requested;

        if (commandQueue.pop(requested)) {
            std::cout << "\n[CLIENT] POP command: "
                      << Protocol::lightToString(requested)
                      << std::endl;

            std::cout << "[CLIENT] Queue AFTER pop: ";
            commandQueue.printQueue();

            std::cout << "[CLIENT] Applying command\n";
            trafficLight.set(requested);

        } else {
            std::cout << "[CLIENT] No command in queue, auto next\n";
            trafficLight.next();
        }
    } 
}