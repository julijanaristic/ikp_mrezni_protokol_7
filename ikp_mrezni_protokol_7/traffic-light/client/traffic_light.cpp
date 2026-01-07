#include "traffic_light.h"

using Protocol::Light;

TrafficLight::TrafficLight() : current(Light::RED) {}

Light TrafficLight::getCurrent() const {
    return current;
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