#include "server_socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <iostream>

ServerSocket::ServerSocket(int port)
    :port(port), serverFd(-1) {}

ServerSocket::~ServerSocket(){
    if(serverFd != -1){
        close(serverFd);
    }
}

void ServerSocket::start() {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd < 0){
        throw std::runtime_error("Socket creation failed");
    }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0){
        throw std::runtime_error("Bind failed");
    }

    if(listen(serverFd, 10) < 0){
        throw std::runtime_error("Listen failed");
    }

    std::cout << "[SERVER] Listening on port " << port << std::endl;
}

int ServerSocket::acceptClient(){
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);
    if(clientFd < 0){
        throw std::runtime_error("Accept failed");
    }

    std::cout << "[SERVER] Client connected (fd=" << clientFd << ")\n";
    return clientFd;
}

int ServerSocket::getSocketFd() const {
    return serverFd;
}