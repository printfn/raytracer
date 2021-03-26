#include "Cube.h"

#include "utility.h"

void testPlane(
        std::vector<RayIntersection> &result,
        RayIntersection hit,
        const Ray& ray,
        const Ray& inverseRay,
        const Transform& transform,
        size_t d1,
        size_t d2,
        size_t axis,
        double neg) {
    double a0 = inverseRay.point(axis);
    double da = inverseRay.direction(axis);
    if (std::abs(da) < epsilon) {
        return;
    }
    double t = (neg - a0) / da;
    if (t > 0) {
        hit.point = Point{inverseRay.point + t * inverseRay.direction};

        if (hit.point(d1) < 1 && hit.point(d1) > -1 && hit.point(d2) < 1 && hit.point(d2) > -1) {

            hit.point = transform.apply(hit.point);
            if (axis == 2) {
                hit.normal = transform.apply(Normal{0, 0, neg});
            } else if (axis == 1) {
                hit.normal = transform.apply(Normal{0, neg, 0});
            } else if (axis == 0) {
                hit.normal = transform.apply(Normal{neg, 0, 0});
            }

            hit.distance = (hit.point - ray.point).norm();
            result.push_back(hit);
        }
    }
}

std::vector<RayIntersection> Cube::intersect(const Ray &ray) const {
    std::vector<RayIntersection> result;
    Ray inverseRay = transform.applyInverse(ray);

    RayIntersection hit;
    hit.material = material;

    testPlane(result, hit, ray, inverseRay, transform, 0, 1, 2, 1);
    testPlane(result, hit, ray, inverseRay, transform, 0, 1, 2, -1);
    testPlane(result, hit, ray, inverseRay, transform, 0, 2, 1, 1);
    testPlane(result, hit, ray, inverseRay, transform, 0, 2, 1, -1);
    testPlane(result, hit, ray, inverseRay, transform, 1, 2, 0, 1);
    testPlane(result, hit, ray, inverseRay, transform, 1, 2, 0, -1);
    return result;
}
