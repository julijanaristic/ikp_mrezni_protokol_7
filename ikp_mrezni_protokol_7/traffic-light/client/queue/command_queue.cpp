#include "command_queue.h"

CommandQueue::CommandQueue() : head(nullptr), tail(nullptr) {}

CommandQueue::~CommandQueue() {
    while(head) {
        Command* tmp = head;
        head = head->next;
        delete tmp;
    }
}

void CommandQueue::push(Protocol::Light light){
    Command* cmd = new Command{light, nullptr};

    std::lock_guard<std::mutex> lock(mtx);

    if(!tail) {
        head = tail = cmd;
    } else {
        tail->next = cmd;
        tail = cmd;
    }

    cv.notify_one();
}

bool CommandQueue::pop(Protocol::Light& out){
    std::lock_guard<std::mutex> lock(mtx);

    if(!head)
        return false;

    Command* cmd = head;
    out = cmd->requested;

    head = head->next;
    if(!head)
        tail = nullptr;

    delete cmd;
    return true;
}

bool CommandQueue::empty() {
    std::lock_guard<std::mutex> lock(mtx);
    return head == nullptr;
}

void CommandQueue::printQueue(){
    std::lock_guard<std::mutex> lock(mtx);

    std::cout << "[CLIENT][QUEUE] ";

    if(!head){
        std::cout << "(empty)";
    } else {
        Command* curr = head;
        while(curr) {
            std::cout << Protocol::lightToString(curr->requested);
            if(curr->next)
                std::cout << " -> ";
            curr = curr->next;
        }
    }

    std::cout << std::endl;
}

Protocol::Light CommandQueue::getLastOrCurrent(Protocol::Light current){
    std::lock_guard<std::mutex> lock(mtx);
    if(!tail){
        return current;
    }
    return tail->requested;
}