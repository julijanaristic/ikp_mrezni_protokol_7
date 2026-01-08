#include "client_socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <Stdexcept>

ClientSocket::ClientSocket(const std::string& serverIp, int port): serverIp(serverIp), port(port), socketFd(-1) {}

ClientSocket::~ClientSocket() {
    if (socketFd != -1)
        close(socketFd);
}

void ClientSocket::connectToServer() {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd < 0)
        throw std::runtime_error("Socket creation failed");

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0)
        throw std::runtime_error("Invalid address");

    if (connect(socketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw std::runtime_error("Connection failed");
}

int ClientSocket::getFd() const {
    return socketFd;
}

bool ClientSocket::sendMessage(const std::string& msg) {
    return send(socketFd, msg.c_str(), msg.size(), 0) > 0;
}

bool ClientSocket::receiveMessage(std::string& out) {
    char byffer[256];
    memset(buffer, 0, sizeof(buffer));

    int bytes = recv(socketFd, buffer, sizeof(buffer), 0);
    if (bytes <= 0)
        return false;

    out = buffer;
    return true;
}