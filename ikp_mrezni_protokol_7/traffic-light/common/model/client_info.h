#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

struct ClientInfo {
    int socketFd;
    bool active;
};

#endif