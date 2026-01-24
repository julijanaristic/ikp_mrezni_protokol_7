#include "socket/server_socket.h"
#include "accept/server_accept.h"
#include "thread_pool/thread_pool.h"
#include "../common/protocol/message.h"
#include "../common/protocol/message_codec.h"

#include <iostream>
#include <unistd.h>
#include <string>

int main() {
    const int PORT = 8080;

    try{
        ServerSocket server(PORT);
        server.start();

        ThreadPool pool(4);
        AcceptThread acceptThread(server.getSocketFd(), pool);

        acceptThread.start();
        
        std::cout << "[SERVER] Running...\n";
        std::cout << "Type 'exit' to shutdown server\n";

        std::string cmd;
        while(std::getline(std::cin, cmd)){
            if(cmd == "exit")
                break;

            Protocol::Message msg(-1, Protocol::MessageType::COMMAND, cmd);
            acceptThread.broadcast(Protocol::serialize(msg));
        }
        
        std::cout << "[SERVER] Shutting down...\n";

        Protocol::Message shutdown(-1, Protocol::MessageType::SHUTDOWN, "");
        acceptThread.broadcast(Protocol::serialize(shutdown));

        sleep(1);

        acceptThread.stop();
        pool.shutdownAll();
        
    } catch (const std::exception& e){
        std::cerr << "[SERVER ERROR] " << e.what() << std::endl;
        return 1;
    }
    return 0;
}