#include "DirectionalLightSource.h"

#include <utility>

DirectionalLightSource::DirectionalLightSource(const Colour &colour, Direction direction) :
        LightSource(colour), direction_(std::move(direction)) {
}

Colour DirectionalLightSource::getIlluminationAt(const Point &) const {
    return colour_; // intensity does not decay with distance
}


double DirectionalLightSource::getDistanceToLight(const Point &) const {
    return 0; // the distance is always infinite
}

Direction DirectionalLightSource::getLightDirection(const Point &) const {
    return direction_; // the direction is always the same
}
