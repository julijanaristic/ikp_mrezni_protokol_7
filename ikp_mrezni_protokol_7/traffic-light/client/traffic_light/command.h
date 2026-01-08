#ifndef COMMAND_H
#define COMMAND_H

#include "../common/light_types.h"

struct Command {
    Protocol::Light requested;
    Command* next;
};

#endif