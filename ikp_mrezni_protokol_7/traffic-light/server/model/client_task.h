#ifndef CLIENT_TASK_H
#define CLIENT_TASK_H

struct ClientTask {
    int socketFd;
    ClientTask* next;
};

#endif