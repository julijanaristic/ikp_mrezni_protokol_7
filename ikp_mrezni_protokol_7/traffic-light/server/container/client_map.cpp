#include "client_map.h"
#include <string>

ClientMap::ClientMap() {
    for(int i=0; i<TABLE_SIZE; i++)
        table[i] = nullptr;
}

ClientMap::~ClientMap() {
    for(int i=0; i<TABLE_SIZE; i++) {
        Node* curr = table[i];
        while(curr) {
            Node* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
    }
}

int ClientMap::hash(int key) {
    return key % TABLE_SIZE;
}

void ClientMap::insert(int key, const ClientInfo& value) {
    int idx = hash(key);
    Node* newNode = new Node{key, value, table[idx]};
    table[idx] = newNode;
}

void ClientMap::remove(int key) {
    int idx = hash(key);
    Node* curr = table[idx];
    Node* prev = nullptr;

    while(curr) {
        if (curr->key == key) {
            if (prev) prev->next = curr->next;
            else table[idx] = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

ClientInfo* ClientMap::find(int key) {
    int idx = hash(key);
    Node* curr = table[idx];

    while(curr) {
        if(curr->key == key)
            return &curr->value;
        curr = curr->next;
    }
    return nullptr;
}

bool ClientMap::exists(int key) {
    return find(key) != nullptr;
}

//omogucava bezbednu iteraciju bez izlaganja interne strukture
void ClientMap::forEach(void (*func)(ClientInfo&)){
    for(int i = 0; i < TABLE_SIZE; i++){
        Node* curr = table[i];
        while(curr){
            func(curr->value);
            curr = curr->next;
        }
    }
}