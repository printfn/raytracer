#include "RaytracerOptions.h"

#include <cstring>
#include <iostream>
#include <optional>

RaytracerOptions::RaytracerOptions() {}

bool RaytracerOptions::showGUI() {
#if USE_X11
    return showGUI_;
#else
    return false;
#endif
}

int RaytracerOptions::threads() {
    return threads_;
}

std::optional<std::string> RaytracerOptions::outputFilename() {
    return outputFilename_;
}

std::string RaytracerOptions::inputFilename() {
    return inputFilename_;
}

void RaytracerOptions::disableGui() {
    showGUI_ = false;
}

std::optional<RaytracerOptions> RaytracerOptions::fromArgs(int argc, const char *argv[]) {
    auto options = RaytracerOptions{};
    std::optional<std::string> filename;

    for (int i = 1; i < argc; ++i) {
        auto arg = argv[i];
        if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
            std::cerr << "Usage: raytracer [FLAGS] input.txt" << std::endl << std::endl;
            std::cerr << "Flags:" << std::endl;
            std::cerr << "    --no-gui                     Disable the X11 GUI" << std::endl;
            std::cerr << "    -j, --jobs <N>               Specify the number of threads" << std::endl;
            std::cerr << "    -o, --output <filename.jxl>  Override the image output filename" << std::endl;
            return std::nullopt;
        }
        if (!strcmp(arg, "--nogui") || !strcmp(arg, "--no-gui")) {
            options.showGUI_ = false;
            continue;
        }
        if (!strcmp(arg, "-j") || !strcmp(arg, "--jobs")) {
            if (++i >= argc) {
                std::cerr << "Use -j or --jobs to specify the number of threads (e.g. -j 2)." << std::endl;
                return std::nullopt;
            }
            options.threads_ = atoi(argv[i]);
            continue;
        }
        if (!strcmp(arg, "-o") || !strcmp(arg, "--output")) {
            if (++i >= argc) {
                std::cerr << "Use -o or --output to override the output filename." << std::endl;
                return std::nullopt;
            }
            options.outputFilename_ = argv[i];
            continue;
        }
        if (!filename) {
            filename = arg;
        } else {
            std::cerr << "Cannot render multiple input files at once" << std::endl;
            return std::nullopt;
        }
    }

    if (!filename) {
        std::cerr << "No input file specified" << std::endl;
        return std::nullopt;
    }

    options.inputFilename_ = *filename;
    return {options};
}
