#include "../client/client_socket/client_socket.h"
#include "../common/protocol/message.h"
#include "../common/protocol/message_codec.h"

#include <thread>
#include <vector>
#include <iostream>

void runClient() {
    try {
        ClientSocket sock("127.0.0.1", 8080);
        sock.connectToServer();

        // primi ASSIGNED_ID
        std::string raw;
        sock.receiveMessage(raw);

        Protocol::Message hello = Protocol::deserialize(raw);
        int myId = hello.clientId;

        // šalje 50 ACK poruka sa PRAVIM clientId
        for (int i = 0; i < 50; i++) {
            Protocol::Message ping(
                myId,
                Protocol::MessageType::ACK,
                ""
            );
            sock.sendMessage(Protocol::serialize(ping));
        }
    }
    catch (...) {
        std::cerr << "[CLIENT] Error\n";
    }
}


int main() {
    const int CLIENTS = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < CLIENTS; i++) {
        threads.emplace_back(runClient);
    }

    for (auto& t : threads)
        t.join();

    std::cout << "[TEST1 CLIENT] Finished\n";
    return 0;
}
