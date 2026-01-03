#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include "message_types.h"
#include "light_types.h"

namespace Protocol {

    const std::string MSG_COMMAND = "COMMAND";
    const std::string MSG_ACK = "ACK";
    const std::string MSG_ERROR = "ERROR";
    const std::string MSG_SHUTDOWN = "SHUTDOWN";
    const std::string MSG_CLIENT_EXIT = "CLIENT_EXIT";

    const std::string LIGHT_RED = "RED";
    const std::string LIGHT_RED_YELLOW = "RED_YELLOW";
    const std::string LIGHT_GREEN = "GREEN";
    const std::string LIGHT_YELLOW = "YELLOW";

    MessageType parseMessageType(const std::string& msg);
    Light parseLight(const std::string& lightStr);

    std::string lightToString(Light light);
}

#endif // PROTOCOL_H