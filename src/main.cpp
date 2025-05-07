#include "Scene.h"
#include "SceneReader.h"
#include "RaytracerOptions.h"

#include <iostream>

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
int main(int argc, const char *argv[]) {
    auto options = RaytracerOptions::fromArgs(argc, argv);
    if (!options) {
        return 1;
    }

    SceneReader reader{options->inputFilename()};
    auto &scene = reader.getScene();

    if (scene.hasCamera()) {
        scene.render(*options);
    } else {
        std::cerr << "Cannot render a scene with no camera!" << std::endl;
        return 1;
    }

    return 0;
}
