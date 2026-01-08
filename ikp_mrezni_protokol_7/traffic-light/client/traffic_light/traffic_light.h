#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "../common/light_types.h"

#include <chrono>

class TrafficLight {
    public:
        TrafficLight();

        Protocol::Light getCurrent() const;

        void next();
        void waitCurrent();

        bool isValidTransition(Protocol::Light requested) const;

    private:
        Protocol::Light current;

        std::chrono::milliseconds getDuration() const;
};

#endif