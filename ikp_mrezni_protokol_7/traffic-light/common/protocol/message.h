#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "message_types.h"

namespace Protocol {

    struct Message {
        int clientId;
        MessageType type;
        std::string payload;

        Message(): clientId(-1), type(MessageType::UNKNOWN) {}
        Message(int id, MessageType t, const std::string& p): clientId(id), type(t), payload(p) {}
    };
}

#endif