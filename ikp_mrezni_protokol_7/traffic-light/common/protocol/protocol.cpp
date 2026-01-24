#include "protocol.h"
#include <stdexcept>

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

Light parseLight(const std::string& lightStr){
    if(lightStr == LIGHT_RED) return Light::RED;
    if(lightStr == LIGHT_RED_YELLOW) return Light::RED_YELLOW;
    if(lightStr == LIGHT_GREEN) return Light::GREEN;
    if(lightStr == LIGHT_YELLOW) return Light::YELLOW;

    throw std::runtime_error("Unknown light");
}

std::string lightToString(Light light){
    switch(light){
        case Light::RED: return LIGHT_RED;
        case Light::RED_YELLOW: return LIGHT_RED_YELLOW;
        case Light::GREEN: return LIGHT_GREEN;
        case Light::YELLOW: return LIGHT_YELLOW;
        default: return "UNKNOWN";
    }
}

}