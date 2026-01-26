#ifndef CLIENT_MAP_H
#define CLIENT_MAP_H

#include "../model/client_info.h"

class ClientMap {
    public:
        ClientMap();
        ~ClientMap();

        void insert(int key, const ClientInfo& value);
        void remove(int key);
        ClientInfo* find(int key);
        bool exists(int key);
        void forEach(void (*func)(ClientInfo&));

    private:
        struct Node {
            int key; //clientId
            ClientInfo value;
            Node* next;
        };

        static const int TABLE_SIZE = 64;
        Node* table[TABLE_SIZE];
        
        int hash(int key);

};

#endif