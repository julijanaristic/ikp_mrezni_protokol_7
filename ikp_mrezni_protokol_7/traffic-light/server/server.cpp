#include "socket/server_socket.h"
#include "accept/server_accept.h"
#include "thread_pool/thread_pool.h"
#include "../common/protocol/message.h"
#include "../common/protocol/message_codec.h"

#include <iostream>
#include <unistd.h>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

enum class ServerMode {
    NORMAL,
    STRESS_TEST_1,
    STRESS_TEST_2
};

int main() {
    const int PORT = 8080;

    //ServerMode mode = ServerMode::STRESS_TEST_1;
    //ServerMode mode = ServerMode::NORMAL;
    ServerMode mode = ServerMode::STRESS_TEST_2;

    try {
        ServerSocket server(PORT);
        server.start();

        ThreadPool pool(4);
        AcceptThread acceptThread(server.getSocketFd(), pool);
        acceptThread.start();

        std::cout << "[SERVER] Running...\n";

        // NORMAL MODE
        if (mode == ServerMode::NORMAL) {
            std::cout << "Type 'exit' to shutdown server\n";

            std::string cmd;
            while (std::getline(std::cin, cmd)) {
                if (cmd == "exit")
                    break;

                Protocol::Message msg(
                    -1,
                    Protocol::MessageType::COMMAND,
                    cmd
                );
                acceptThread.broadcast(Protocol::serialize(msg));
            }
        }

        // STRESS TEST 1
        else if (mode == ServerMode::STRESS_TEST_1) {
            std::cout << "[TEST1] Stress test 1 started\n";
            std::cout << "[TEST1] Waiting for clients...\n";

            // Server samo stoji i prima konekcije
            std::this_thread::sleep_for(std::chrono::seconds(15));

            std::cout << "[TEST1] Finished\n";
        }

        // STRESS TEST 2
        else if (mode == ServerMode::STRESS_TEST_2) {
            std::cout << "[TEST2] Sending 1000 commands\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));

            std::vector<std::string> sequence = {
                "RED_YELLOW",
                "GREEN",
                "YELLOW",
                "RED"
            };

            for (int i = 0; i < 1000; i++) {
                Protocol::Message cmd(
                    -1,
                    Protocol::MessageType::COMMAND,
                    sequence[i % sequence.size()]
                );
                acceptThread.broadcast(Protocol::serialize(cmd));
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }

            std::cout << "[TEST2] Commands sent\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        // SHUTDOWN
        std::cout << "[SERVER] Shutting down...\n";

        Protocol::Message shutdown(
            -1,
            Protocol::MessageType::SHUTDOWN,
            ""
        );
        acceptThread.broadcast(Protocol::serialize(shutdown));

        sleep(1);

        acceptThread.stop();
        pool.shutdownAll();

    } catch (const std::exception& e) {
        std::cerr << "[SERVER ERROR] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
