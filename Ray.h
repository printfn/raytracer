#pragma once

#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include "Point.h"
#include "Direction.h"

/// Rays are represented as a starting Point and a Direction.
class Ray {
public:
    /// The starting Point for the Ray.
    Point point;

    /// The Direction for the Ray.
    Direction direction;
};

#endif
