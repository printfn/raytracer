#ifndef RAYTRACEROPTIONS_H_INCLUDED
#define RAYTRACEROPTIONS_H_INCLUDED

#include <optional>
#include <string>

/// Class to store command-line options for the raytracer
class RaytracerOptions {
public:
    static std::optional<RaytracerOptions> fromArgs(int argc, const char *argv[]);
    bool showGUI();
    int threads();
    std::optional<std::string> outputFilename();
    std::string inputFilename();
    void disableGui();
private:
    explicit RaytracerOptions();
    bool showGUI_ = true;
    int threads_ = 1;
    std::optional<std::string> outputFilename_{};
    std::string inputFilename_;
};

#endif // RAYTRACEROPTIONS_H_INCLUDED
