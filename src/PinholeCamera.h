#pragma once

#ifndef PINHOLE_CAMERA_H_INCLUDED
#define PINHOLE_CAMERA_H_INCLUDED

#include "Camera.h"

/**
 * \brief PinholeCamera class.
 *
 * PinholeCamera implements a common Camera model that gives ideal perspective
 * projections. The camera is defined by a point (the camera centre) and an image 
 * plane at some distance along the Z-axis (the focal distance). The projection of
 * a 3D point into the image is the intersection of the line from the camera centre 
 * to the 3D point with the image plane.
 */
class PinholeCamera : public Camera {

public:

    /**
     * \brief PinholeCamera constructor.
     *
     * This creates a new PinholeCamera with the camera centre at the origin,
     * looking along the positive Z-axis. The focal length is provided as 
     * a parameter (default: 1 unit). The position and orientation of a Camera
     * can be altered via its transform member.
     *
     * \param f The focalLength of the new camera.
     */
    explicit PinholeCamera(double f = 1);

    virtual ~PinholeCamera() = default;

    /** \brief Generate a ray for a given image plane co-ordinate
     *
     * For a untransformed PinholeCamera the ray starts at the origin
     * and passes through (x, y, focalLength).
     *
     * \param x the horizontal location
     * \param y the vertical location
     * \return The Ray that passes from the Camera through (x,y) in the image plane.
     */
    Ray castRay(double x, double y) const override;

    double focalLength; //!< The distance from the camera centre to the image plane.

private:

};

#endif
