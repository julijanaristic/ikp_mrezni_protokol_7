#ifndef COMMAND_H
#define COMMAND_H

#include "../../common/protocol/light_types.h"

struct Command {
    Protocol::Light requested;
    Command* next;
};

#endif