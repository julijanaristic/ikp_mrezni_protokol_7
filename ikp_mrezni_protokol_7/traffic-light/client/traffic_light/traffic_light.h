#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "../../common/protocol/light_types.h"
#include <mutex>

#include <chrono>

class TrafficLight {
    public:
        TrafficLight();

        Protocol::Light getCurrent() const;

        void next();
        void waitCurrent();
        void set(Protocol::Light l);

        bool isValidTransition(Protocol::Light current, Protocol::Light requested) const;

    private:
        Protocol::Light current;
        mutable std::mutex mtx;

        std::chrono::milliseconds getDuration() const;
};

#endif