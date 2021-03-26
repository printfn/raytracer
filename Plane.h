#pragma once

#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "Object.h"

/**
* \brief Class for Plane objects.
*
* This class provides an Object which is a plane centred at the origin, extending \f$\pm 1\f$ units along the \f$X\f$ and \f$Y\f$ axes.
*
*/
class Plane : public Object {

public:

    /** \brief Plane default constructor.
    *
    * A newly constructed Plane is centred at the origin, and extends \f$\pm 1\f$ units along the \f$X\f$ and \f$Y\f$ axes.
    * It may be moved, rotated, and scaled through its transform member.
    */
    Plane() = default;

    /** \brief Plane-Ray intersection computation.
    *
    * The intersection of a Ray with a Plane comes down to determining when the
    * \f$Z\f$ component of the Ray is zero. If that lies within the bounds of the 
    * plane there is an intersection.
    *
    * \param ray The Ray to intersect with this Sphere.
    * \return A list (std::vector) of intersections, which may be empty.
    */
    std::vector<RayIntersection> intersect(const Ray &ray) const override;

};

#endif // PLANE_H_INCLUDED
