#include "Mesh.h"
#include "utility.h"
#include <fstream>
#include <sstream>
#include <cstring>

Mesh::Mesh() : Object{}, _faces{}, _min{infinity, infinity, infinity}, _max{-infinity, -infinity, -infinity} {
}

bool startsWith(const std::string &s, const std::string &prefix) {
    // the second argument to rfind ensures that we only look for the
    // prefix at index 0
    return s.rfind(prefix, 0) == 0;
}

Mesh::Mesh(const std::string& filename) : Mesh{} {
    std::fstream fin{filename};
    if (!fin.is_open()) {
        std::cerr << "Failed to open file " << filename << ": " << strerror(errno) << std::endl;
        return;
    }
    std::string line;
    std::vector<Point> vertices;
    std::vector<Normal> normals;
    while (std::getline(fin, line)) {
        std::stringstream strstream{line};
        std::string token;
        if (!(strstream >> token)) continue;
        // comment: discard rest of line
        if (token[0] == '#') continue;
        // ignore lines starting 'mtllib' (defining/using materials)
        if (token == "mtllib" || token == "usemtl") continue;
        // ignore groups and objects
        if (token == "g" || token == "o") continue;
        // ignore texture coordinates
        if (token == "vt") continue;
        // ignore object smoothing settings - smoothing is always enabled
        if (token == "s") continue;
        if (token == "v") {
            // vertex
            Point pt;
            strstream >> pt(0);
            strstream >> pt(1);
            strstream >> pt(2);
            if (pt(0) < _min(0)) _min(0) = pt(0);
            if (pt(0) > _max(0)) _max(0) = pt(0);
            if (pt(1) < _min(1)) _min(1) = pt(1);
            if (pt(1) > _max(1)) _max(1) = pt(1);
            if (pt(2) < _min(2)) _min(2) = pt(2);
            if (pt(2) > _max(2)) _max(2) = pt(2);
            vertices.push_back(pt);
        } else if (token == "vn") {
            // vertex normal
            Normal normal;
            strstream >> normal(0);
            strstream >> normal(1);
            strstream >> normal(2);
            normals.push_back(normal);
        } else if (token == "f") {
            // face element
            std::vector<Point> faceVertices;
            std::vector<Normal> faceNormals;
            int idx0 = -1, idx1 = -1, idx2 = -1;
            char ch;
            while (strstream >> idx0) {
                if (!(strstream >> ch) || ch != '/') {
                    std::cerr << "Error parsing .obj file\n" << std::endl;
                    return;
                }
                if (strstream.peek() != '/') {
                    strstream >> idx1;
                }
                if (!(strstream >> ch) || ch != '/') {
                    std::cerr << "Error parsing .obj file\n" << std::endl;
                    return;
                }
                strstream >> idx2;
                // .obj files use 1-based indices
                faceVertices.push_back(vertices[idx0 - 1]);
                faceNormals.push_back(normals[idx2 - 1]);
            }
            if (faceVertices.size() == 3) {
                Triangle t1;
                t1.v1 = faceVertices[0];
                t1.v2 = faceVertices[1];
                t1.v3 = faceVertices[2];
                t1.n1 = faceNormals[0];
                t1.n2 = faceNormals[1];
                t1.n3 = faceNormals[2];
                _faces.push_back(t1);
            } else if (faceVertices.size() == 4) {
                Triangle t1, t2;
                t1.v1 = faceVertices[0];
                t1.v2 = faceVertices[1];
                t1.v3 = faceVertices[2];
                t1.n1 = faceNormals[0];
                t1.n2 = faceNormals[1];
                t1.n3 = faceNormals[2];

                t2.v1 = faceVertices[0];
                t2.v2 = faceVertices[2];
                t2.v3 = faceVertices[3];
                t2.n1 = faceNormals[0];
                t2.n2 = faceNormals[2];
                t2.n3 = faceNormals[3];

                _faces.push_back(t1);
                _faces.push_back(t2);
            } else {
                std::cerr << "Error: Face with " << faceVertices.size() << " vertices!" << std::endl;
            }
        } else {
            std::cerr << "Unknown token: " << token << std::endl;
        }
    }
    std::cout << "Successfully read " << _faces.size() << " faces from " << filename << std::endl;
    std::cout << "Bounding box: ("
        << _min(0) << ", "
        << _min(1) << ", "
        << _min(2) << "), ("
        << _max(0) << ", "
        << _max(1) << ", "
        << _max(2) << ")" << std::endl;
    fin.close();
}

std::vector<RayIntersection> Mesh::intersect(const Ray &ray) const {
    Ray inverseRay = transform.applyInverse(ray);
    if (useBoundingBox) {
        Cube boundingBox{}; // -1 to 1
        boundingBox.transform.translate(1, 1, 1); // 0 to 2
        boundingBox.transform.scale(0.5); // 0 to 1
        auto scale = _max - _min;
        boundingBox.transform.scale(scale(0), scale(1), scale(2));
        boundingBox.transform.translate(Direction{_min});
        auto boundingBoxIntersections = boundingBox.intersect(inverseRay);
        if (boundingBoxIntersections.empty()) {
            return boundingBoxIntersections; // no intersections: return empty vector
        }
    }
    std::vector<RayIntersection> result;
    for (auto &&triangle : _faces) {
        auto intersection = triangle.intersects(inverseRay);
        if (intersection.has_value()) {
            auto hit = *intersection;
            hit.point = transform.apply(hit.point);
            hit.normal = transform.apply(hit.normal);
            hit.material = material;
            hit.distance = (hit.point - ray.point).norm();
            result.push_back(hit);
        }
    }
    return result;
}
