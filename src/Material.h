#pragma once

#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "Colour.h"

/// Class defining Material colours and properties.
class Material {

public:
    Material() = default;

    /// Colour of Material under white ambient light. Usually, but not always, the same as diffuseColour.
    Colour ambientColour = Colour{1, 1, 1};     

    /// Colour of Material under direct white light. Usually, but not always, the same as ambientColour.
    Colour diffuseColour = Colour{1, 1, 1};

    /// Colour of Material's specular highlights. If this is zero then there are no highlights.
    Colour specularColour = Colour{0, 0, 0};

    /// 'Hardness' of Material's specular highlights - high values give small, sharp highlights.
    double specularExponent = 1;

    /// Colour of reflected rays under direct white light. If this is zero then there are no reflections.
    Colour mirrorColour = Colour{0, 0, 0};

    /// Opacity of the object, between 0 and 1. Fully opaque objects have a value of 1.
    double opacity = 1;

    /// Index of refraction of the object. The default value is 1, glass has a value of ~1.5.
    double refractiveIndex = 1;
};

#endif
