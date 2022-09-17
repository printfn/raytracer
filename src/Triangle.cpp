#include "Triangle.h"
#include "Ray.h"
#include "utility.h"
#include "RayIntersection.h"

std::optional<RayIntersection> Triangle::intersects(const Ray &inverseRay) const {
    // Möller–Trumbore intersection algorithm
    auto edge1 = v2 - v1;
    auto edge2 = v3 - v1;
    auto h = inverseRay.direction.cross(edge2);
    auto a = edge1.dot(h);
    if (std::abs(a) < epsilon) {
        return {}; // the ray is parallel to the triangle
    }
    auto f = 1.0 / a;
    auto s = inverseRay.point - v1;
    auto u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) {
        return {};
    }
    auto q = s.cross(edge1);
    auto v = f * inverseRay.direction.dot(q);
    if (v < 0.0 || u + v > 1.0) {
        return {};
    }
    auto t = f * edge2.dot(q);
    if (t > epsilon) {
        RayIntersection hit;
        hit.point = Point{inverseRay.point + t * inverseRay.direction};
        hit.normal = Normal{(1 - u - v) * n1 + u * n2 + v * n3};
        return std::make_optional(hit);
    }
    return {};
}
