#include "PinholeCamera.h"

PinholeCamera::PinholeCamera(double f) : Camera(), focalLength(f) {
}

Ray PinholeCamera::castRay(double x, double y) const {
    Ray ray;
    ray.point = Point{Matrix::zero(3, 1)};
    ray.direction(0) = x;
    ray.direction(1) = y;
    ray.direction(2) = focalLength;
    return transform.apply(ray);
}
