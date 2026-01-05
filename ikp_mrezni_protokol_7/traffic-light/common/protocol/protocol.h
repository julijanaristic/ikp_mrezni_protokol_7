#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include "message_types.h"
#include "light_types.h"

namespace Protocol {

    const std::string MSG_COMMAND;
    const std::string MSG_ACK;
    const std::string MSG_ERROR;
    const std::string MSG_SHUTDOWN;
    const std::string MSG_CLIENT_EXIT;

    const std::string LIGHT_RED;
    const std::string LIGHT_RED_YELLOW;
    const std::string LIGHT_GREEN;
    const std::string LIGHT_YELLOW;

    MessageType parseMessageType(const std::string& msg);
    Light parseLight(const std::string& lightStr);

    std::string lightToString(Light light);
}

#endif // PROTOCOL_H