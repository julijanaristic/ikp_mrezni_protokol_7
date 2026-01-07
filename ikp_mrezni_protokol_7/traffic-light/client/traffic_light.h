#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "../common/light_types.h"

class TrafficLight {
    public:
        TrafficLight();

        Protocol::Light getCurrent() const;
        void next();

    private:
        Protocol::Light current;
};

#endif