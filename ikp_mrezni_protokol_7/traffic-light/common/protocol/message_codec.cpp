#include "message_codec.h"
#include <sstream>

namespace Protocol {
    std::string messageTypeTopString(MessageType type) {
        switch (type) {
            case MessageType::COMMAND: return "COMMAND";
            case MessageType::ACK: return "ACK";
            case MessageType::ERROR: return "ERROR";
            case MessageType::SHUTDOWN: return "SHUTDOWN";
            case MessageType::CLIENT_EXIT: return "CLIENT_EXIT";
            default: return "UNKNOWN";
        }
    }

    MessageType stringToMessageType(const std::string& s) {
        if (s == "COMMAND") return MessageType::COMMAND;
        if (s == "ACK") return MessageType::ACK;
        if (s == "ERROR") return MessageType::ERROR;
        if (s == "SHUTDOWN") return MessageType::SHUTDOWN;
        if (s == "CLIENT_EXIT") return MessageType::CLIENT_EXIT;
        return MessageType::UNKNOWN;
    }

    std::string serialize(const Message& msg) {
        return std::to_string(msg.clientId) + "|" + messageTypeTopString(msg.type) + "|" + msg.payload + "\n";
    }

    Message deserialize(const std::string& raw) {
        Message msg;
        std::stringstream ss(raw);
        std::string part;

        std::getline(ss, part, '|');
        msg.clientId = std::stoi(part);

        std::getline(ss, part, '|');
        msg.type = stringToMessageType(part);

        std::getline(ss, msg.payload);

        return msg;
    }
}