#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include "message_types.h"
#include "light_types.h"

namespace Protocol {

    extern const std::string MSG_COMMAND;
    extern const std::string MSG_ACK;
    extern const std::string MSG_ERROR;
    extern const std::string MSG_SHUTDOWN;
    extern const std::string MSG_CLIENT_EXIT;

    extern const std::string LIGHT_RED;
    extern const std::string LIGHT_RED_YELLOW;
    extern const std::string LIGHT_GREEN;
    extern const std::string LIGHT_YELLOW;

    Light parseLight(const std::string& lightStr);

    std::string lightToString(Light light);
}

#endif // PROTOCOL_H