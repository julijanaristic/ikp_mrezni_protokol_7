#ifndef MESSAGE_CODEC_H
#define MESSAGE_CODEC_H

#include "message.h"

namespace Protocol {
    std::string serialize(const Message& msg);
    Message deserialize(const std::string& raw);
}

#endif