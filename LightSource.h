#pragma once

#ifndef LIGHT_SOURCE_H_INCLUDED
#define LIGHT_SOURCE_H_INCLUDED

#include "Colour.h"
#include "Ray.h"

/// Abstract base class for LightSources.
class LightSource {

public:

    /** \brief Determine how much light reaches a Point from this LightSource.
     *
     * The amount of light emitted by a LightSource is determined by its colour property,
     * but this generally varies across the Scene. This method returns a scaling factor to
     * apply to the colour of the light at a given Point in the Scene.
     *
     * \param point The Point at which light is measured.
     * \return The illumination that reaches the Point.
     */
    virtual Colour getIlluminationAt(const Point &point) const = 0;

    /** \brief Compute the distance from a Point to this LightSource
     *
     * \param point The Point from which the distance is measured.
     * \return The distance from the Point to the LightSource
     */
    virtual double getDistanceToLight(const Point &point) const = 0;

    /** \brief Determine the direction from which light from this LightSource falls
     *
     * \param point The Point at which to measure the direction.
     * \return The direction from which light falls on the Point.
     */
    virtual Direction getLightDirection(const Point &point) const = 0;

protected:

    /** \brief LightSource constructor.
     *
     * \param colour The Colour of this LightSource's illumination.
     */
    explicit LightSource(const Colour &colour);

    Colour colour_; //!< The Colour of this LightSource's illumination.
};

#endif // LIGHT_SOURCE_H_INCLUDED
