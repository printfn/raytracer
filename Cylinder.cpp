#include "Cylinder.h"
#include "utility.h"

std::vector<RayIntersection> Cylinder::intersect(const Ray &ray) const {
    std::vector<RayIntersection> result;

    Ray inverseRay = transform.applyInverse(ray);

    const Point &p = inverseRay.point;
    const Direction &d = inverseRay.direction;

    double a = d.dot(d);
    double b = 2 * d.dot(p);
    double c = p.dot(p) - 1;

    RayIntersection hit;
    hit.material = material;

    double b2_4ac = b * b - 4 * a * c;
    double t;
    switch (sign(b2_4ac)) {
        case -1:
            // No intersections
            break;
        case 0:
            // One intersection
            t = -b / (2 * a);
            if (t > 0) {
                // Intersection is in front of the ray's start point
                hit.point = transform.apply(Point(p + t * d));
                hit.normal = transform.apply(Normal(p + t * d));
                if (hit.normal.dot(ray.direction) > 0) {
                    hit.normal = Normal{-hit.normal};
                }
                hit.distance = (hit.point - ray.point).norm();
                result.push_back(hit);
            }
            break;
        case 1:
            // Two intersections
            t = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
            if (t > 0) {
                // Intersection is in front of the ray's start point
                hit.point = transform.apply(Point(p + t * d));
                hit.normal = transform.apply(Normal(p + t * d));
                if (hit.normal.dot(ray.direction) > 0) {
                    hit.normal = Normal{-hit.normal};
                }
                hit.distance = (hit.point - ray.point).norm();
                result.push_back(hit);
            }

            t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
            if (t > 0) {
                // Intersection is in front of the ray's start point
                hit.point = transform.apply(Point(p + t * d));
                hit.normal = transform.apply(Normal(p + t * d));
                if (hit.normal.dot(ray.direction) > 0) {
                    hit.normal = Normal{-hit.normal};
                }
                hit.distance = (hit.point - ray.point).norm();
                result.push_back(hit);
            }
            break;
        default:
            // Shouldn't be possible, but just in case
            std::cerr << "Something's wrong - sign(x) should be -1, +1 or 0" << std::endl;
            exit(-1);
            break;
    }
    double z0 = inverseRay.point(2);
    double dz = inverseRay.direction(2);
    double dy = inverseRay.direction(1);
    double dx = inverseRay.direction(0);

    if ((std::abs(dz) < epsilon) || (std::abs(dy) < epsilon) || (std::abs(dx) < epsilon)) {
        return result;
    }

    double t1 = (-1 - z0) / dz;
    double t2 = (1 - z0) / dz;

    hit.material = material;

    if (t1 > 0) {
        hit.point = Point{inverseRay.point + t1 * inverseRay.direction};

        if ((hit.point(0) < 1 && hit.point(0) > -1) && (hit.point(1) < 1 && hit.point(1) > -1)) {

            hit.point = transform.apply(Point(hit.point));

            hit.normal = Normal{0, 0, 1};
            hit.normal = transform.apply(Normal(hit.normal));

            if (hit.normal.dot(ray.direction) > 0) {
                hit.normal = Normal{-hit.normal};
            }

            hit.distance = (hit.point - ray.point).norm();
            result.push_back(hit);
        }
    }

    if (t2 > 0) {
        hit.point = Point{inverseRay.point + t2 * inverseRay.direction};

        if ((hit.point(0) < 1 && hit.point(0) > -1) && (hit.point(1) < 1 && hit.point(1) > -1)) {

            hit.point = transform.apply(Point(hit.point));

            hit.normal = Normal{0, 0, 1};
            hit.normal = transform.apply(Normal(hit.normal));

            if (hit.normal.dot(ray.direction) > 0) {
                hit.normal = Normal{-hit.normal};
            }

            hit.distance = (hit.point - ray.point).norm();
            result.push_back(hit);
        }
    }
    hit.distance = (hit.point - ray.point).norm();
    result.push_back(hit);
    return result;
}
