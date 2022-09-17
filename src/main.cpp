#include "Scene.h"
#include "SceneReader.h"
#include "RaytracerOptions.h"

#include <iostream>
#include <cstring>

/** \brief Create a Scene, read input from files, and render the Scene.
 *
 * The main function creates an empty Scene then uses a SceneReader
 * to add information based on input files specified as command line
 * arguments.
 *
 * The scene is then rendered and saved to file, as long as there
 * is a Camera specified.
 * 
 */
int main(int argc, char *argv[]) {
    auto options = RaytracerOptions{};
    std::optional<std::string> filename;

    for (int i = 1; i < argc; ++i) {
        auto arg = argv[i];
        if (!strcmp(arg, "--nogui") || !strcmp(arg, "--no-gui")) {
            options.showGUI = false;
            continue;
        }
        if (!strcmp(arg, "-j") || !strcmp(arg, "--jobs")) {
            if (++i >= argc) {
                std::cerr << "Use -j or --jobs to specify the number of threads (e.g. -j 2)." << std::endl;
                return 1;
            }
            options.threads = atoi(argv[i]);
            continue;
        }
        if (!strcmp(arg, "-o") || !strcmp(arg, "--output")) {
            if (++i >= argc) {
                std::cerr << "Use -o or --output to override the output filename." << std::endl;
                return 1;
            }
            options.outputFilename = argv[i];
            continue;
        }
        if (!filename) {
            filename = arg;
        } else {
            std::cerr << "Cannot render multiple input files at once" << std::endl;
            return 1;
        }
    }

    if (!filename) {
        std::cerr << "No input file specified" << std::endl;
        return 1;
    }

    SceneReader reader{filename.value()};
    auto &scene = reader.getScene();

    if (scene.hasCamera()) {
        scene.render(options);
    } else {
        std::cerr << "Cannot render a scene with no camera!" << std::endl;
        return 1;
    }

    return 0;
}
