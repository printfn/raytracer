#include "AmbientLightSource.h"

AmbientLightSource::AmbientLightSource(const Colour &colour) : LightSource(colour) {
}

Colour AmbientLightSource::getIlluminationAt(const Point &) const {
    return colour_;
}

double AmbientLightSource::getDistanceToLight(const Point &) const {
    return -1;
}

Direction AmbientLightSource::getLightDirection(const Point &) const {
    return Direction(0, 0, 0);
}
