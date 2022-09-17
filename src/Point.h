#pragma once

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "Vector.h"

/** \brief Class for Point Vectors.
 *
 * A Point can be seen as either a 3-Vector or a homogeneous 4-Vector.
 * In this implementation a Point is a 3-Vector, and the Transform 
 * class deals with the homogeneous form. Point is essentially a thin
 * wrapper around Vector, which ensures that the Vector has 3 elements.
 *
 * Having separate types for Point, Direction, and Normal, means that 
 * it is possible to distinguish them when passing them to  Transform.apply() etc.
 */
class Point : public Vector {

public:
    Point();

    Point(double x, double y, double z);

    /** \brief Point from Vector constructor.
     *
     * Arithmetic operations on Point objects use the Vector implementations.
     * This means that the result is a Vector, and this allows them to be converted to 
     * Point objects.
     *
     * \param vector The Vector to copy to \c this.
     */
    explicit Point(const Vector &vector);

    /** \brief Point from Matrix constructor.
     *
     * Some operations on Point objects use the Matrix implementations.
     * This means that the result is a Matrix, and this allows them to be converted to 
     * Point objects.
     *
     * \param matrix The Matrix to copy to \c this.
     */
    explicit Point(const Matrix &matrix);

};

#endif
