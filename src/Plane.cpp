#include "Plane.h"

#include "utility.h"

std::vector<RayIntersection> Plane::intersect(const Ray &ray) const {
    std::vector<RayIntersection> result;
    Ray inverseRay = transform.applyInverse(ray);

    // compute the intersection between the ray and the plane
    double z0 = inverseRay.point(2);
    double dz = inverseRay.direction(2);

    // make sure we're not dividing by 0
    if (std::abs(dz) < epsilon) {
        return result;
    }

    // find where on the ray it's hitting the plane
    double t = -z0 / dz;

    // if the plane is behind the starting point of the ray: no intersection
    if (t <= 0) {
        return result;
    }

    // compute the actual intersection
    RayIntersection hit;
    hit.point = Point{inverseRay.point + t * inverseRay.direction};
    hit.material = material;

    // ensure that the x and y values of the hitpoint lie in the range [-1,1]
    if ((hit.point(0) < 1 && hit.point(0) > -1) && (hit.point(1) < 1 && hit.point(1) > -1)) {
        // transform the point back to where it should be (in case the plane has been scaled, rotated or translated)
        hit.point = transform.apply(hit.point);

        // set the normal vector at the intersection, also based on the transformation
        hit.normal = transform.apply(Normal(0, 0, 1));

        // ensure the normal is pointing in the right direction
        if (hit.normal.dot(ray.direction) > 0) {
            hit.normal = Normal{-hit.normal};
        }

        hit.distance = (hit.point - ray.point).norm();
        result.push_back(hit);
    }
    return result;
}
