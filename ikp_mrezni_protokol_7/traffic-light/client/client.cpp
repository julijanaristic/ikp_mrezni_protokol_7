#include "client_socket/client_socket.h"
#include "traffic_light/traffic_light.h"
#include "queue/command_queue.h"
#include "../common/protocol/protocol.h"
#include "../common/protocol/message_codec.h"
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

        int clientId = -1;

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

            Protocol::Message msgObj = Protocol::deserialize(msg);

            //handshake
            if(msgObj.type == Protocol::MessageType::ACK && 
                msgObj.payload == "ASSIGNED_ID"){

                    clientId = msgObj.clientId;
                    std::cout << "[CLIENT] Assigned ID = " << clientId << std::endl;
                    continue;
                }

            if (msgObj.type == Protocol::MessageType::COMMAND) {
                try {
                    Protocol::Light requested = Protocol::parseLight(msgObj.payload);
                    Protocol::Light base = commandQueue.empty() ? trafficLight.getCurrent() : commandQueue.getLastOrCurrent(trafficLight.getCurrent());

                    if (trafficLight.isValidTransition(base, requested)) {
                        commandQueue.push(requested);
                        commandQueue.printQueue();
                        Protocol::Message ack(
                            clientId,
                            Protocol::MessageType::ACK,
                            ""
                        );
                        socket.sendMessage(Protocol::serialize(ack));
                    }
                    else {
                        Protocol::Message err(
                            clientId,
                            Protocol::MessageType::ERROR,
                            "Invalid transition"
                        );
                        socket.sendMessage(Protocol::serialize(err));
                    }
                } catch (...) {
                    Protocol::Message err(
                        clientId,
                        Protocol::MessageType::ERROR,
                        "Unknown light command"
                    );
                    socket.sendMessage(Protocol::serialize(err));
                }
            }
            else if (msgObj.type == Protocol::MessageType::SHUTDOWN) {
                Protocol::Message exit(
                    clientId,
                    Protocol::MessageType::CLIENT_EXIT,
                    ""
                );
                socket.sendMessage(Protocol::serialize(exit));
                running = false;
            }
            else {
                Protocol::Message err(
                    clientId,
                    Protocol::MessageType::ERROR,
                    "Unknown command"
                );
                socket.sendMessage(Protocol::serialize(err));
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