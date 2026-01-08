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