#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>

class ClientSocket {
    public:
        ClientSocket(const std::string& serverIp, int port);
        ~ClientSocket();

        void connectToServer();
        int getFd() const;

        bool sendMessage(const std::string& msg);
        bool receiveMessage(std::string& out);

    private:
        std::string serverIp;
        int port;
        int socketFd;
};

#endif
