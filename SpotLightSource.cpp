#include "SpotLightSource.h"
#include "utility.h"
#include <utility>

SpotLightSource::SpotLightSource(const Colour &colour, Point location, Direction direction, double angle)
        :
        LightSource(colour), location_(std::move(location)), direction_(std::move(direction)), angle_(angle) {

}

Colour SpotLightSource::getIlluminationAt(const Point &point) const {
    auto directionToPoint = point - location_;
    auto distance = directionToPoint.norm();
    // compute cos of the angle between the light's actual direction and the direction to the target point
    auto cosOfAngle = directionToPoint.dot(direction_) / distance / direction_.norm();
    // if we're outside the light cone
    if (cosOfAngle < cos(angle_ * Deg2Rad * 2)) {
        // object is outside the cone, so return black
        return Colour{};
    }
    if (distance < epsilon) distance = epsilon;
    // attenuate light by distance
    return (1.0 / (distance * distance)) * colour_;
}

double SpotLightSource::getDistanceToLight(const Point &point) const {
    return (point - location_).norm();
}

Direction SpotLightSource::getLightDirection(const Point &point) const {
    return Direction{point - location_};
}
