#include "traffic_light.h"
#include <thread>

using Protocol::Light;
using namespace std::chrono;

TrafficLight::TrafficLight() : current(Light::RED) {}

Light TrafficLight::getCurrent() const {
    return current;
}

milliseconds TrafficLight::getDuration() const {
    switch (current) {
        case Light::RED:
            return seconds(5);
        
        case Light::RED_YELLOW:
            return seconds(2);
        
        case Light::GREEN:
            return seconds(5);
        
        case Light::YELLOW:
            return seconds(2);

        default:
            return seconds(1);
    }
}

void TrafficLight::waitCurrent() {
    std::this_thread::sleep_for(getDuration());
}

void TrafficLight::next() {
    switch (current) {
        case Light::RED:
            current = Light::RED_YELLOW;
            break;
        
        case Light::RED_YELLOW:
            current = Light::GREEN;
            break;
        
        case Light::GREEN:
            current = Light::YELLOW;
            break;
        
        case Light::YELLOW:
            current = Light::RED;
            break;
        
        default:
            current = Light::RED;
            break;
    }
}

bool TrafficLight::isValidTransition(Protocol::Light current, Protocol::Light requested) const {
    using Protocol::Light;
    
    switch(current) {
        case Light::RED:
            return requested == Light::RED_YELLOW;
        
        case Light::RED_YELLOW:
            return requested == Light::GREEN;

        case Light::GREEN:
            return requested == Light::YELLOW;
        
        case Light::YELLOW:
            return requested == Light::RED;

        default:
            return false;
    }
}