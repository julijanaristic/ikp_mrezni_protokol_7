#include "../client/client_socket/client_socket.h"
#include "../common/protocol/message.h"
#include "../common/protocol/message_codec.h"

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        ClientSocket sock("127.0.0.1", 8080);
        sock.connectToServer();

        std::string raw;
        sock.receiveMessage(raw);
        Protocol::Message hello = Protocol::deserialize(raw);

        int myId = hello.clientId;
        std::cout << "[CLIENT] Connected, id = " << myId << "\n";
        
        int processed = 0;

        while (true) {
            std::string rawMsg;
            sock.receiveMessage(rawMsg);

            Protocol::Message msg = Protocol::deserialize(rawMsg);

            if (msg.type == Protocol::MessageType::COMMAND) {
                processed++;

                std::cout << "[CLIENT] COMMAND #" << processed << ": " << msg.payload << std::endl;

                std::this_thread::sleep_for(std::chrono::seconds(1));
            } 
            else if (msg.type == Protocol::MessageType::SHUTDOWN) {
                std::cout << "[CLIENT] Shutdown received]\n";
                break;
            }
        }

        std::cout << "[CLIENT] Processed total commands: " << processed << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[CLIENT ERROR] " << e.what() << std::endl;
    }

    return 0;
}