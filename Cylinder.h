#pragma once

#ifndef CYLINDER_H_INCLUDED
#define CYLINDER_H_INCLUDED

#include "Object.h"

/// Class for Cylinder objects.
class Cylinder : public Object {

public:

    Cylinder() = default;

    std::vector<RayIntersection> intersect(const Ray &ray) const override;

private:

};

#endif // CYLINDER_H_INCLUDED
