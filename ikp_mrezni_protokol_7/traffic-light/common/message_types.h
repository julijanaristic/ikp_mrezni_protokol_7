#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

namespace Protocol {
    enum class MessageType {
        COMMAND,
        ACK,
        ERROR,
        SHUTDOWN,
        CLIENT_EXIT,
        UNKNOWN
    };
}
#endif
