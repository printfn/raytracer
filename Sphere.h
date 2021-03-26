#pragma once

#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include "Object.h"

/**
 * \brief Class for Sphere objects.
 * 
 * This class provides an Object which is a sphere centred at the origin with radius 1.
 * 
 */
class Sphere : public Object {

public:

    Sphere() = default;

    std::vector<RayIntersection> intersect(const Ray &ray) const override;

};

#endif // SPHERE_H_INCLUDED
