#pragma once

#ifndef POINT_LIGHT_SOURCE_H_INCLUDED
#define POINT_LIGHT_SOURCE_H_INCLUDED

#include "LightSource.h"

/**
 * \brief Light emitted from a Point.
 *
 * A PointLightSource represents light emitted from a point in the Scene.
 * The amount of light that reaches any given part of the scene follows
 * a \f$1/r^2\f$ law, so more distant Objects receive less illumination.
 */
class PointLightSource : public LightSource {

public:

    /** \brief PointLightSource constructor. 
     *
     * This creates a LightSource with a specified Colour at a given Location.
     *
     * \param colour The Colour of the PointLightSource
     * \param location The location of the PointLightSource
     **/
    PointLightSource(const Colour &colour, Point location);

    virtual ~PointLightSource() = default;

    /** \brief Determine how much light reaches a Point from this PointLightSource.
     *
     * The colour property determines the basic amount of light emitted by a 
     * PointLightSource. This is the illumination received at distance 1 from 
     * the PointLightSource, but it will appear brighter or darker if the illuminated
     * Point is closer or further away. The amount of light received by a Point
     * is scaled by \f$1/d^2\f$, where \f$d\f$ is the distance between the Point
     * and the PointLightSource.
     *
     * \param point The Point at which light is measured.
     * \return The illumination that reaches the Point.
     */
    Colour getIlluminationAt(const Point &point) const override;

    /** \brief Compute the distance from a Point to this LightSource
    *
    * For a PointLightSource this is the distance between the Point and the LightSource's location.
    *
    * \param point The Point from which the distance is measured.
    * \return The distance from the Point to the LightSource
    */
    double getDistanceToLight(const Point &point) const override;

    /** \brief Determine the direction from which light from this LightSource falls
    *
    * Note that this is the direction <em>from<\em> the light <em>to</em> point.
    *
    * \param point The Point at which to measure the direction.
    * \return The direction from which light falls on the Point.
    */
    Direction getLightDirection(const Point &point) const override;

private:

    Point location_; //!< Location of this PointLightSource
};

#endif
