#include "PointLightSource.h"

#include <utility>

#include "utility.h"

PointLightSource::PointLightSource(const Colour &colour, Point location) :
        LightSource(colour), location_(std::move(location)) {

}

Colour PointLightSource::getIlluminationAt(const Point &point) const {
    double distance = (location_ - point).norm();
    if (distance < epsilon) distance = epsilon;
    return (1.0 / (distance * distance)) * colour_;
}


double PointLightSource::getDistanceToLight(const Point &point) const {
    return (location_ - point).norm();
}

Direction PointLightSource::getLightDirection(const Point &point) const {
    return Direction{point - location_};
}
