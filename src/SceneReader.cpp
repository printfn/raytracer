#include "SceneReader.h"

#include "Camera.h"
#include "PinholeCamera.h"

#include "LightSource.h"
#include "AmbientLightSource.h"
#include "DirectionalLightSource.h"
#include "PointLightSource.h"
#include "SpotLightSource.h"

#include "Object.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Mesh.h"
#include "utility.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <sstream>
#include <filesystem>
#include <cstring>
#include <random>

static std::string outputFilename(const std::string& filename) {
    auto path = std::filesystem::path{filename};
    return path.filename().replace_extension(".jxl");
}

SceneReader::SceneReader(const std::string& filename) :
        scene_(std::make_unique<Scene>(outputFilename(filename))), startLine_(0) {
    read(filename);
}

Scene &SceneReader::getScene() {
    return *scene_;
}

void SceneReader::parseTokenBlock(std::queue<std::string> &tokenBlock) {
    std::string blockType = tokenBlock.front();
    tokenBlock.pop();
    if (blockType == "SCENE") {
        parseSceneBlock(tokenBlock);
    } else if (blockType == "CAMERA") {
        parseCameraBlock(tokenBlock);
    } else if (blockType == "OBJECT") {
        parseObjectBlock(tokenBlock);
    } else if (blockType == "LIGHT") {
        parseLightBlock(tokenBlock);
    } else if (blockType == "MATERIAL") {
        parseMaterialBlock(tokenBlock);
    } else {
        std::cerr << "Unexpected block type '" << blockType << "' starting on line " << startLine_ << std::endl;
        exit(-1);
    }
}

void SceneReader::read(const std::string &filename) {
    std::ifstream fin{filename};
    if (!fin.is_open()) {
        std::cerr << "Failed to open file " << filename << ": " << strerror(errno) << std::endl;
        exit(-1);
    }

    std::cout << "Reading scene from " << filename << std::endl;

    std::string line;
    int lineNumber = 0;
    startLine_ = 0;
    std::queue<std::string> tokenBlock;
    while (std::getline(fin, line)) {
        ++lineNumber;
        std::transform(line.begin(), line.end(), line.begin(), toupper);
        std::stringstream strstream{line};
        std::string token;
        while (strstream >> token) {
            if (token[0] == '#') {
                // A comment - skip the rest of the line
                break;
            } else if (token == "END") {
                parseTokenBlock(tokenBlock);
            } else {
                if (tokenBlock.empty()) {
                    startLine_ = lineNumber;
                }
                tokenBlock.push(token);
            }
        }
    }

    if (!tokenBlock.empty()) {
        std::cerr << "Unexpected end of file in " << filename << std::endl;
        exit(-1);
    }

    fin.close();

}


void SceneReader::parseSceneBlock(std::queue<std::string> &tokenBlock) {
    while (!tokenBlock.empty()) {
        std::string token = tokenBlock.front();
        tokenBlock.pop();
        if (token == "BACKGROUNDCOLOUR") {
            scene_->backgroundColour = parseColour(tokenBlock);
        } else if (token == "RENDERSIZE") {
            scene_->renderWidth = int(parseNumber(tokenBlock));
            scene_->renderHeight = int(parseNumber(tokenBlock));
        } else if (token == "SCALERESOLUTION") {
            auto resolutionScale = parseNumber(tokenBlock);
            scene_->renderWidth *= resolutionScale;
            scene_->renderHeight *= resolutionScale;
        } else if (token == "FSAA") {
            scene_->fsaa = int(parseNumber(tokenBlock));
        } else if (token == "RENDERNORMALS") {
            scene_->renderNormals = true;
        } else if (token == "RAYDEPTH") {
            scene_->maxRayDepth = int(parseNumber(tokenBlock));
        } else {
            std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
            exit(-1);
        }
    }
}

double SceneReader::parseNumber(std::queue<std::string> &tokenBlock) const {
    std::string token = tokenBlock.front();
    tokenBlock.pop();
    char *endPtr;
    double result = strtod(token.c_str(), &endPtr);
    if (endPtr != token.c_str() + token.length()) {
        std::cerr << "Expected a number but found '" << token << "' in block starting on line " << startLine_
                  << std::endl;
        exit(-1);
    }
    return result;
}

Colour SceneReader::parseColour(std::queue<std::string> &tokenBlock) {
    double red = parseNumber(tokenBlock);
    double green = parseNumber(tokenBlock);
    double blue = parseNumber(tokenBlock);
    return Colour::with_reverse_gamma_correction(red, green, blue);
}

void SceneReader::parseCameraBlock(std::queue<std::string> &tokenBlock) {
    // Make a new camera
    std::string cameraType = tokenBlock.front();
    tokenBlock.pop();
    std::shared_ptr<Camera> camera;
    if (cameraType == "PINHOLECAMERA") {
        double focalLength = parseNumber(tokenBlock);
        camera = std::make_shared<PinholeCamera>(focalLength);
        scene_->setCamera(camera);
    } else {
        std::cerr << "Unexpected camera type '" << cameraType << "' in block starting on line " << startLine_
                  << std::endl;
        exit(-1);
    }

    // Parse camera details
    while (!tokenBlock.empty()) {
        std::string token = tokenBlock.front();
        tokenBlock.pop();
        if (token == "ROTATE") {
            std::string axis = tokenBlock.front();
            tokenBlock.pop();
            double angle = parseNumber(tokenBlock) * Deg2Rad;
            if (axis == "X") {
                camera->transform.rotateX(angle);
            } else if (axis == "Y") {
                camera->transform.rotateY(angle);
            } else if (axis == "Z") {
                camera->transform.rotateZ(angle);
            } else {
                std::cerr << "Unexpected axis '" << axis << "' in block starting on line " << startLine_ << std::endl;
                exit(-1);
            }
        } else if (token == "TRANSLATE") {
            double tx = parseNumber(tokenBlock);
            double ty = parseNumber(tokenBlock);
            double tz = parseNumber(tokenBlock);
            camera->transform.translate(tx, ty, tz);
        } else if (token == "SCALE") {
            double s = parseNumber(tokenBlock);
            camera->transform.scale(s);
        } else if (token == "SCALE3") {
            double sx = parseNumber(tokenBlock);
            double sy = parseNumber(tokenBlock);
            double sz = parseNumber(tokenBlock);
            camera->transform.scale(sx, sy, sz);
        } else {
            std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
            exit(-1);
        }

    }

}

double randomInRange(double min, double max) {
    std::random_device rd;
    std::mt19937 generator{rd()};
    std::uniform_real_distribution<> dist{min, max};
    return dist(generator);
}

Point randomPointInRadius(Point location, double radius) {
    if (radius == 0) {
        return location;
    }
    // generate a uniform random point in a sphere
    double theta = randomInRange(0, TAU);
    double v = randomInRange(0, 1);
    double phi = acos((2*v)-1);
    double r = pow(randomInRange(0,1), 1/3);
    double dx = r * sin(phi) * cos(theta);
    double dy = r * sin(phi) * sin(theta);
    double dz = r * cos(phi);
    return {
            location(0) + dx * radius,
            location(1) + dy * radius,
            location(2) + dz * radius,
    };
}

void SceneReader::parseLightBlock(std::queue<std::string> &tokenBlock) {
    std::string lightType = tokenBlock.front();
    tokenBlock.pop();
    Point location;
    Colour colour;
    Direction direction;
    double angle;
    double radius = 0;
    int samples = 1;

    while (!tokenBlock.empty()) {
        std::string token = tokenBlock.front();
        tokenBlock.pop();
        if (token == "LOCATION") {
            location(0) = parseNumber(tokenBlock);
            location(1) = parseNumber(tokenBlock);
            location(2) = parseNumber(tokenBlock);
        } else if (token == "COLOUR") {
            colour = parseColour(tokenBlock);
        } else if (token == "DIRECTION") {
            direction(0) = parseNumber(tokenBlock);
            direction(1) = parseNumber(tokenBlock);
            direction(2) = parseNumber(tokenBlock);
        } else if (token == "ANGLE") {
            angle = parseNumber(tokenBlock);
        } else if (token == "RADIUS") {
            radius = parseNumber(tokenBlock);
        } else if (token == "SAMPLES") {
            samples = static_cast<int>(parseNumber(tokenBlock));
        } else {
            std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
            exit(-1);
        }
    }

    if (lightType == "POINTLIGHT") {
        for (int sample = 0; sample < samples; ++sample) {
            Point sampleLocation = randomPointInRadius(location, radius);
            scene_->addLight(std::make_shared<PointLightSource>(colour / samples, sampleLocation));
        }
    } else if (lightType == "SPOTLIGHT") {
        for (int sample = 0; sample < samples; ++sample) {
            Point sampleLocation = randomPointInRadius(location, radius);
            scene_->addLight(std::make_shared<SpotLightSource>(colour / samples, sampleLocation, direction, angle));
        }
    } else if (lightType == "AMBIENTLIGHT") {
        scene_->addLight(std::make_shared<AmbientLightSource>(colour));
    } else if (lightType == "DIRECTIONALLIGHT") {
        scene_->addLight(std::make_shared<DirectionalLightSource>(colour, direction));
    } else {
        std::cerr << "Unexpected light type '" << lightType << "' in block starting on line " << startLine_
                  << std::endl;
        exit(-1);
    }
}

void SceneReader::parseObjectBlock(std::queue<std::string> &tokenBlock) {
    std::string objectType = tokenBlock.front();
    tokenBlock.pop();
    std::shared_ptr<Object> object;
    if (objectType == "SPHERE") {
        object = std::make_shared<Sphere>();
    } else if (objectType == "CUBE") {
        object = std::make_shared<Cube>();
    } else if (objectType == "PLANE") {
        object = std::make_shared<Plane>();
    } else if (objectType == "CYLINDER") {
        object = std::make_shared<Cylinder>();
    } else if (objectType == "MESH") {
        std::string filename = tokenBlock.front();
        tokenBlock.pop();
        std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
        std::cout << "Reading .obj file " << filename << std::endl;
        object = std::make_shared<Mesh>(filename);
    } else {
        std::cerr << "Unexpected object type '" << objectType << "' in block starting on line " << startLine_
                  << std::endl;
        exit(-1);
    }
    scene_->addObject(object);

    // Parse object details
    while (!tokenBlock.empty()) {
        std::string token = tokenBlock.front();
        tokenBlock.pop();
        if (token == "ROTATE") {
            std::string axis = tokenBlock.front();
            tokenBlock.pop();
            double angle = Deg2Rad * parseNumber(tokenBlock);
            if (axis == "X") {
                object->transform.rotateX(angle);
            } else if (axis == "Y") {
                object->transform.rotateY(angle);
            } else if (axis == "Z") {
                object->transform.rotateZ(angle);
            } else {
                std::cerr << "Unexpected axis '" << axis << "' in block starting on line " << startLine_ << std::endl;
                exit(-1);
            }
        } else if (token == "TRANSLATE") {
            double tx = parseNumber(tokenBlock);
            double ty = parseNumber(tokenBlock);
            double tz = parseNumber(tokenBlock);
            object->transform.translate(tx, ty, tz);
        } else if (token == "SCALE") {
            double s = parseNumber(tokenBlock);
            object->transform.scale(s);
        } else if (token == "SCALE3") {
            double sx = parseNumber(tokenBlock);
            double sy = parseNumber(tokenBlock);
            double sz = parseNumber(tokenBlock);
            object->transform.scale(sx, sy, sz);
        } else if (token == "MATERIAL") {
            std::string materialName = tokenBlock.front();
            tokenBlock.pop();
            auto material = materials_.find(materialName);
            if (material == materials_.end()) {
                std::cerr << "Undefined material '" << materialName << "' in block starting on line " << startLine_
                          << std::endl;
                exit(1);
            } else {
                object->material = material->second;
            }
        } else if (token == "COLOUR") {
            Colour objColour = parseColour(tokenBlock);
            object->material.ambientColour = objColour;
            object->material.diffuseColour = objColour;
        } else if (token == "AMBIENT") {
            object->material.ambientColour = parseColour(tokenBlock);
        } else if (token == "DIFFUSE") {
            object->material.diffuseColour = parseColour(tokenBlock);
        } else if (token == "SPECULAR") {
            object->material.specularColour = parseColour(tokenBlock);
            object->material.specularExponent = parseNumber(tokenBlock);
        } else if (token == "MIRROR") {
            object->material.mirrorColour = parseColour(tokenBlock);
        } else if (token == "OPACITY") {
            object->material.opacity = parseNumber(tokenBlock);
        } else if (token == "REFRACTIVEINDEX") {
            object->material.refractiveIndex = parseNumber(tokenBlock);
        } else if (token == "DISABLEBOUNDINGBOX") {
            object->useBoundingBox = false;
        } else {
            std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
            exit(-1);
        }

    }
}

void SceneReader::parseMaterialBlock(std::queue<std::string> &tokenBlock) {
    std::string materialName = tokenBlock.front();
    tokenBlock.pop();
    if (materials_.find(materialName) == materials_.end()) {
        materials_[materialName] = Material();
    } else {
        std::cerr << "Warning: duplicate definition of material '" << materialName
                  << "' found in block starting on line " << startLine_ << std::endl;
    }
    Material &material = materials_.find(materialName)->second;

    while (!tokenBlock.empty()) {
        std::string token = tokenBlock.front();
        tokenBlock.pop();
        if (token == "COLOUR") {
            Colour objColour = parseColour(tokenBlock);
            material.ambientColour = objColour;
            material.diffuseColour = objColour;
        } else if (token == "AMBIENT") {
            material.ambientColour = parseColour(tokenBlock);
        } else if (token == "DIFFUSE") {
            material.diffuseColour = parseColour(tokenBlock);
        } else if (token == "SPECULAR") {
            material.specularColour = parseColour(tokenBlock);
            material.specularExponent = parseNumber(tokenBlock);
        } else if (token == "MIRROR") {
            material.mirrorColour = parseColour(tokenBlock);
        } else if (token == "OPACITY") {
            material.opacity = parseNumber(tokenBlock);
        } else if (token == "REFRACTIVEINDEX") {
            material.refractiveIndex = parseNumber(tokenBlock);
        } else {
            std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
            exit(-1);
        }

    }
}
