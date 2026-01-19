#include "client_socket/client_socket.h"
#include "traffic_light/traffic_light.h"
#include "queue/command_queue.h"
#include "../common/protocol/protocol.h"
#include "execution_thread/execution_thread.h"

#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> running(true);

int main() {
    try {
        ClientSocket socket("127.0.0.1", 8080);
        socket.connectToServer();

        TrafficLight trafficLight;
        CommandQueue commandQueue;
        ExecutionThread execThread(trafficLight, commandQueue);

        execThread.start();

        while (running) {
            std::string msg;
            
            if (!socket.receiveMessage(msg)){
                std::cout << "[CLIENT] Server disconnected\n";
                running = false;
                execThread.stop();
                break;
            }
        
            if (!msg.empty() && msg.back() == '\n')
                msg.pop_back();

            Protocol::MessageType type = Protocol::parseMessageType(msg);

            if (type == Protocol::MessageType::COMMAND) {
                size_t space = msg.find(' ');
                std::string lightStr = msg.substr(space + 1);

                Protocol::Light requested = Protocol::parseLight(lightStr);
                Protocol::Light base = commandQueue.empty() ? trafficLight.getCurrent() : commandQueue.getLastOrCurrent(trafficLight.getCurrent());

                if (trafficLight.isValidTransition(base, requested)) {
                    commandQueue.push(requested);
                    commandQueue.printQueue();
                    socket.sendMessage("ACK\n");
                }
                else {
                    socket.sendMessage("ERROR Invalid transition\n");
                }
            }
            else if (type == Protocol::MessageType::SHUTDOWN) {
                socket.sendMessage("CLIENT_EXIT\n");
                running = false;
            }
            else {
                socket.sendMessage("ERROR Unknown command\n");
            }
        }

        running = false;
        execThread.stop();
    }
    catch (const std::exception& e) {
        std::cerr << "[CLIENT ERROR] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}