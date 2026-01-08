#include "socket/client_socket.h"
#include "traffic_light/traffic_light.h"
#include "queue/command_queue.h"
#include "../common/protocol.h"

#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> running(true);

void executionLoop(TrafficLight& light, CommandQueue& queue) {
    while (running) {
        light.waitCurrent();
        light.next();

        Protocol::Light nextCmd;
        if (queue.pop(nextCmd))
            light.next();
    }
}

int main() {
    try {
        ClientSocket socket("127.0.0.1", 8080);
        socket.connectToServer();

        TrafficLight trafficLight;
        CommandQueue commandQueue;

        std::trhead execThread(executionLoop, std::ref(trafficLight), std::ref(commandQueue));

        while (running) {
            std::string msg;
            if (!socket.receiveMessage(msg))
                break;
        
            if (!msg.empty() && msg.back() == '\n')
                msg.pop_back();

            MessageType type = Protocol::parseMessageType(msg);

            if (type == MessageType::COMMAND) {
                size_t space = msg.find(' ');
                std::string lightStr = msg.substr(space + 1);

                Protocol::Light requested = Protocol::parseLight(lightStr);

                if (trafficLight.isValidTransition(trafficLight.getCurrent(), requested)) {
                    commandQueue.push(requested);
                    socket.sendMessage("ACK\n");
                }
                else {
                    socket.sendMessage("ERROR Invalid transition\n");
                }
            }
            else if (type == MessageType::SHUTDOWN) {
                socket.sendMessage("CLIENT_EXIT\n");
                running = false;
            }
            else {
                socket.sendMessage("ERROR Unknown command\n");
            }
        }

        running = false;
        execThread.join();
    }
    catch (const std::exception& e) {
        std::cerr << "[CLIENT ERROR] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}