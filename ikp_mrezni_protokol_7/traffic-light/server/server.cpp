#include "socket/server_socket.h"
#include "accept/server_accept.h"
#include "thread_pool/thread_pool.h"

#include <iostream>
#include <unistd.h>

int main() {
    const int PORT = 8080;

    try{
        ServerSocket server(PORT);
        server.start();

        ThreadPool pool(4);
        AcceptThread acceptThread(server.getSocketFd(), pool);

        acceptThread.start();
        
        std::cout << "[SERVER] Running...\n";
        
        while(true){
            sleep(1);
        }
    } catch (const std::exception& e){
        std::cerr << "[SERVER ERROR] " << e.what() << std::endl;
        return 1;
    }
    return 0;
}