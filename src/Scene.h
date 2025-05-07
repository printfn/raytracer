#pragma once

#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Camera.h"
#include "Colour.h"
#include "LightSource.h"
#include "Material.h"
#include "NonCopyable.h"
#include "Object.h"
#include "Ray.h"
#include "RayIntersection.h"

class RaytracerOptions;
class ImageDisplay;

/**
 * \brief A Scene to be ray traced.
 *
 * Internally it provides the methods required to render an image, such as intersecting a Ray
 * with the Objects in the Scene, and computing the Colour from a Ray.
 *
 */
class Scene : private NonCopyable {

public:

    /** \brief Scene constructor.
     * This creates an empty Scene, with a black background and no ambient light.
     * By default the images are rendered at 800x600 pixel resolution, saved
     * to the specified filename, and allow for up to 3 reflected/refracted rays.
     */
    explicit Scene(std::string filename);

    /** \brief Set the Scene's Camera.
     *
     * Since there may be different types of Camera, a smart pointer is 
     * used to refer to the Camera. For example:
     * \code
     *   Scene scene;
     *   std::shared_ptr<PinholeCamera> camera(new PinholeCamera(1));
     *   scene.setCamera(camera);
     * \endcode
     *
     * Note that a Scene has only one Camera, so calling setCamera() will
     * replace any existing camera.
     * 
     * \param camera The new Camera to add.
     */
    void setCamera(std::shared_ptr<Camera> camera) {
        camera_ = std::move(camera);
    }

    /** \brief Add a new Object.
     *
     * Note that the Scene has a collection of Objects, and there is no 
     * way to remove an Object once added. 
     *
     * \param object A \c std::shared_ptr to the new Object to add.
     */
    void addObject(const std::shared_ptr<Object>& object) {
        objects_.push_back(object);
    }

    /** \brief Add a new LightSource.
     *
     * Note that the Scene has a collection of LightSources, and there is no 
     * way to remove an LightSource once added. 
     *
     * \param light A \c std::shared_ptr to the new LightSource to add.
     */
    void addLight(const std::shared_ptr<LightSource>& light) {
        lights_.push_back(light);
    }

    /** \brief Render an image of the Scene.
     * 
     * This method renders an image of the Scene. The size of the
     * image is (renderWidth x renderHeight), and it is saved to a file specified by
     * the Scene's filename property. The format of the file is determined by its
     * extension. 
     *
     * Attempts to render a Scene with no Camera will end badly.
     */
    void render(RaytracerOptions &options) const;

    Colour backgroundColour{}; //!< Colour for any Ray that does not hit an Object.

    unsigned int maxRayDepth = 3; //!< Maximum number of reflected/refracted Rays to trace.
    int fsaa = 1; //!< Anti-aliasing
    bool renderNormals = false;

    bool hasCamera() const;

    int renderWidth = 800;  //!< Width in pixels of the image to render.
    int renderHeight = 600; //!< Height in pixels of the image to render.

private:

    std::shared_ptr<Camera> camera_{};                    //!< Camera to render the image with.
    std::vector<std::shared_ptr<Object>> objects_{};      //!< Collection of Objects in the Scene.
    std::vector<std::shared_ptr<LightSource>> lights_{};  //!< Collection of LightSources in the Scene.
    std::string filename_;                                //!< File to save the image to.

    /** \brief Intersect a Ray with the Objects in a Scene
     *
     * This intersects the Ray with all of the Objects in the Scene and returns
     * the first hit. If there is no hit, then a RayIntersection with infinite distance
     * is returned.
     *
     * \param ray The Ray to intersect with the Objects.
     * \return The first intersection of the Ray with the Scene.
     */
    RayIntersection intersect(const Ray &ray) const;

    /** \brief Compute the Colour seen by a Ray in the Scene.
     * 
     * The Colour seen by a Ray depends on the lighting, the first Object that it
     * hits, and the Material properties of that Object. This method performs these
     * computations and computes the observed Colour. For some Objects it may be necessary
     * to cast other Rays to deal with reflections. This can conceivably recurse forever,
     * so a maximum number of reflections is set.
     *
     * If the Ray does not hit any Object, then the Scene's backgroundColour should be 
     * returned.
     * 
     * \param ray The Ray to intersect with the Objects in the Scene.
     * \param rayDepth The maximum number of reflection Rays that can be cast.
     * \param refractiveIndex The refractive index of the material in which the ray starts.
     * \return The Colour observed by the viewRay.
     */
    Colour computeColour(const Ray &ray, unsigned int rayDepth, double refractiveIndex) const;

    void renderThread(int thread, RaytracerOptions &options, ImageDisplay &display) const;

};

#endif
