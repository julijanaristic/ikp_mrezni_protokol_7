#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

class ServerSocket {
    public:
        ServerSocket(int port);
        ~ServerSocket();

        void start(); //socket + bind + listen
        int acceptClient(); // accept jednog klijenta

        int getSocketFd() const;
    
    private:
        int port;
        int serverFd;
};

#endif