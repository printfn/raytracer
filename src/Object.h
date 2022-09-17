#pragma once

#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "Material.h"
#include "Ray.h"
#include "RayIntersection.h"
#include "Transform.h"

#include <vector>

/// Abstract base class for Objects.
class Object {
public:

    /** \brief Object-Ray intersection computation.
     *
     * Given a Ray in space, this finds the point(s) of intersection with the surface of the 
     * Object. Note that there may be 0, 1, or more intersection points, and so a std::vector
     * of RayIntersections is returned.
     *
     * The details of this depend on the geometry of the particular Object, so this is a 
     * pure virtual method.
     *
     * \param ray The Ray to intersect with this Object.
     * \return A list (std::vector) of intersections, which may be empty.
     */
    virtual std::vector<RayIntersection> intersect(const Ray &ray) const = 0;

    Transform transform{}; //!< A 3D transformation to apply to this Object.

    Material material{}; //!< The colour and reflectance properties of the Object.

    bool useBoundingBox = true; //!< Whether or not to use bounding boxes for this object.

protected:

    Object() = default;

    virtual ~Object() = default;

};

#endif
