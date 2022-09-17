#pragma once

#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#include "Object.h"

/// Class for cube objects. By default a cube extends from -1 to 1 on the x, y and z axes.
class Cube : public Object {

public:

    Cube() = default;

    std::vector<RayIntersection> intersect(const Ray &ray) const override;

};

#endif // CUBE_H_INCLUDED
