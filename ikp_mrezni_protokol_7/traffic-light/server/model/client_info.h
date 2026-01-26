#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

struct ClientInfo {
    int socketFd;
    int clientId;
    bool active;
};

#endif