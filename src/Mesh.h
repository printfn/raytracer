#pragma once

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Object.h"
#include "Point.h"
#include "Cube.h"
#include "Triangle.h"
#include <vector>

/// This class represents a triangle mesh and its bounding box
class Mesh : public Object {

    std::vector<Triangle> _faces;
    Point _min, _max;

public:
    Mesh();
    explicit Mesh(const std::string& filename);

    std::vector<RayIntersection> intersect(const Ray &ray) const override;

};

#endif // MESH_H_INCLUDED
