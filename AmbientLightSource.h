#pragma once

#ifndef AMBIENT_LIGHT_SOURCE_H_INCLUDED
#define AMBIENT_LIGHT_SOURCE_H_INCLUDED

#include "LightSource.h"

/// Ambient light in a scene.
class AmbientLightSource : public LightSource {

public:

    explicit AmbientLightSource(const Colour &colour);

    virtual ~AmbientLightSource() = default;

    Colour getIlluminationAt(const Point &point) const override;
    double getDistanceToLight(const Point &point) const override;
    Direction getLightDirection(const Point &point) const override;

};

#endif // AMBIENT_LIGHT_SOURCE_H_INCLUDED
