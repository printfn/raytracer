#ifndef RAYTRACEROPTIONS_H_INCLUDED
#define RAYTRACEROPTIONS_H_INCLUDED

#include <optional>

/// Struct to store command-line options for the raytracer
struct RaytracerOptions {
    bool showGUI = true;
    int threads = 1;
    std::optional<std::string> outputFilename{};
};

#endif // RAYTRACEROPTIONS_H_INCLUDED
