#pragma once

#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "Point.h"
#include "Normal.h"
#include <optional>

class Ray;
class RayIntersection;

struct Triangle {
    Point v1, v2, v3;
    Normal n1, n2, n3;

    std::optional<RayIntersection> intersects(const Ray &inverseRay) const;
};

#endif // TRIANGLE_H_INCLUDED
