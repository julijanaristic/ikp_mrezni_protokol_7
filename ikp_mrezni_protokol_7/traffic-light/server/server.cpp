#include "server_socket.h"
#include <iostream>
#include <vector>
#include <unistd.h>

int main() {
    const int PORT = 8080;

    try{
        ServerSocket server(PORT);
        server.start();

        std::vector<int> clients;

        while(true){
            int clientFd = server.acceptClient();
            clients.push_back(clientFd);
        }
    } catch (const std::exception& e){
        std::cerr << "[SERVER ERROR] " << e.what() << std::endl;
        return 1;
    }
    return 0;
}