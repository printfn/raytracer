#include "Scene.h"

#include "Colour.h"
#include "ImageDisplay.h"
#include "utility.h"
#include "RaytracerOptions.h"
#include <chrono>
#include <thread>
#include <utility>

Scene::Scene(std::string filename) : filename_(std::move(filename)) {
}

void updateCliProgress(int thread, int &percentage) {
    if (thread != 0) {
        return;
    }
    if (percentage == 0) {
        std::cout << "  0% " << std::flush;
    } else {
        std::cout << "." << std::flush;
        if (percentage == 100) {
            std::cout << std::endl << "100%" << std::endl;
        } else if (percentage % 10 == 0) {
            std::cout << std::endl << " " << percentage << "% " << std::flush;
        }
    }
    ++percentage;
}

void Scene::renderThread(int thread, RaytracerOptions &options, ImageDisplay &display) const {
    const auto w = double(renderWidth);
    const auto h = double(renderHeight);

    auto percentage = 0;

    for (int v = 0; v < renderHeight; ++v) {
        if (v % options.threads() != thread) {
            continue;
        }
        if (thread == 0 && options.showGUI()) {
            try {
                display.refresh();
            } catch (...) {
                std::cerr << "Failed to show GUI. Use --no-gui to disable the X11 GUI." << std::endl;
                options.disableGui();
            }
        }
        if (percentage == 0) {
            updateCliProgress(thread, percentage);
        }
        double progress = 100.0 * v / renderHeight;
        while (thread == 0 && percentage + 1 < progress) {
            updateCliProgress(thread, percentage);
        }
        for (int u = 0; u < renderWidth; ++u) {
            auto colour = Colour{0, 0, 0};
            for (int va = 0; va < fsaa; ++va) {
                for (int ua = 0; ua < fsaa; ++ua) {
                    const auto cu = scale(u * fsaa + ua, 0, renderWidth * fsaa - 1, -1, 1);
                    const auto cv = scale(v * fsaa + va, 0, renderHeight * fsaa - 1, -h / w, h / w);
                    Ray ray = camera_->castRay(cu, cv);
                    colour += computeColour(ray, maxRayDepth, 1);
                }
            }
            display.set(u, v, colour / fsaa / fsaa);
        }
    }
    while (thread == 0 && percentage <= 100) {
        updateCliProgress(thread, percentage);
    }
}

void Scene::render(RaytracerOptions &options) const {
    ImageDisplay display(renderWidth, renderHeight);

    std::string outputFilename;
    if (options.outputFilename()) {
        outputFilename = options.outputFilename().value();
    } else {
        outputFilename = filename_;
    }

    if (options.showGUI() && options.threads() > 1) {
        std::cerr
            << "Warning: Disabling the GUI (using --no-gui) is"
            << " recommended when doing multi-threaded rendering"
            << std::endl;
    }

    std::cout << "Rendering a scene with " << objects_.size() << " objects into " << outputFilename << std::endl;

    const auto startTime = std::chrono::high_resolution_clock::now();

    const auto linesPerThread = renderHeight / options.threads();
    if (linesPerThread < 1) {
        std::cerr << "Too many threads. Please increase resolution or specify fewer threads." << std::endl;
        return;
    }

    std::vector<std::thread> threads;
    for (int thread = 1; thread < options.threads(); ++thread) {
        threads.emplace_back([thread, options, &display, this] {
            auto optionsCopy = options;
            // important: this lambda copied the `thread` and `options` variables
            renderThread(thread, optionsCopy, display);
        });
    }

    // start main thread, which can also update the UI and options if desired
    renderThread(0, options, display);

    for (auto &thread : threads) {
        thread.join();
        if (options.showGUI()) {
            display.refresh();
        }
    }

    const auto finishTime = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime);
    std::cout << "Rendered image in " << duration.count() << " ms" << std::endl;

    std::cout << "Saving rendered image to " << outputFilename << std::endl;
    display.save(outputFilename);

    if (options.showGUI()) {
        std::cout << "Waiting for 5 seconds" << std::endl;
        display.pause(5);
    }
}

RayIntersection Scene::intersect(const Ray &ray) const {
    RayIntersection firstHit;
    firstHit.distance = infinity;
    for (auto &obj : objects_) {
        for (auto &hit : obj->intersect(ray)) {
            if (hit.distance > epsilon && hit.distance < firstHit.distance) {
                firstHit = hit;
            }
        }
    }
    return firstHit;
}

Colour Scene::computeColour(const Ray &ray, unsigned int rayDepth, double refractiveIndex) const {
    RayIntersection hitPoint = intersect(ray);
    if (hitPoint.distance == infinity || rayDepth == 0) {
        return backgroundColour;
    }
    Colour hitColour{};
    if (renderNormals) {
        // this is a special mode that renders all normal vectors as RGB values
        auto normal = hitPoint.normal;
        normal /= normal.norm();
        hitColour = Colour::with_reverse_gamma_correction(
                normal(0) / 2 + 0.5,
                normal(1) / 2 + 0.5,
                normal(2) / 2 + 0.5);
        return hitColour;
    }
    Material &material = hitPoint.material;

    for (const auto& light: lights_) {
        // Compute the influence of this light on the appearance of the hit object.
        if (light->getDistanceToLight(hitPoint.point) < 0) {
            // An ambient light, ignore shadows and add appropriate colour
            hitColour += light->getIlluminationAt(hitPoint.point) * material.ambientColour;
        } else {
            // vector from the hit point back along the view ray
            Vector l = -light->getLightDirection(hitPoint.point);
            l /= l.norm();
            // normal vector from the hit point towards the ith light source
            Vector n = hitPoint.normal;
            n /= n.norm();
            // colour of the light at hitpoint
            Colour illumination = light->getIlluminationAt(hitPoint.point);
            // diffuse colour of the material at the hitpoint
            Colour kd = material.diffuseColour;

            // reflection of l about n
            Vector r = 2 * (n.dot(l)) * n - l;
            r /= r.norm();
            // vector from the hit point back along the view ray
            Vector v = -ray.direction;
            v /= v.norm();
            // specular colour
            Colour ks = material.specularColour;

            double diffusion = std::max(0.0, n.dot(l));

            // generate shadow ray, from the hitpoint towards the light source
            Ray shadowRay;
            shadowRay.point = hitPoint.point;
            shadowRay.direction = Direction{l};

            // test if the shadow ray hits any objects on its way to the light source
            RayIntersection hit = intersect(shadowRay);

            // if the intersection is further away than the light source
            if (hit.distance >= light->getDistanceToLight(hitPoint.point)) {
                auto specular = illumination * ks * pow(r.dot(v), material.specularExponent);
                if (r.dot(v) > 1) {
                    specular = {0, 0, 0};
                }
                hitColour += illumination * kd * diffusion + specular;
            }
        }
    }

    // can be changed by total internal reflection
    auto reflectivity = material.mirrorColour;

    // transparency & refraction (note that this does not currently work correctly)
    if (material.opacity < 1 && hitPoint.entersObject) {
        Vector n = hitPoint.normal;
        Vector v = ray.direction; // a.k.a. l

        v /= v.norm();
        n /= n.norm();

        auto cos_theta_1 = -n.dot(v);

        double materialRI = material.refractiveIndex;
        if (refractiveIndex != 1) {
            // exiting the refractive object
            materialRI = 1;
        }
        auto r = materialRI / refractiveIndex;
        auto c = cos_theta_1;
        auto radicand = 1 - r * r * (1 - c * c);
        if (radicand < 0) {
            // total internal reflection
            reflectivity = Colour{1, 1, 1};
        } else {
            auto v_refract = r * v + (r * c - sqrt(radicand)) * n;
            v_refract /= v_refract.norm();

            auto v_prime = v / std::abs(n.dot(v));
            auto k_r = material.refractiveIndex;
            auto k_f = 1 / sqrt(
                    k_r * k_r * v_prime.norm() * v_prime.norm()
                    - (v_prime + n).norm() * (v_prime + n).norm());
            auto p = k_f * (n + v_prime) - n;
            p /= p.norm();

            Ray transparencyRay;
            transparencyRay.direction = Direction{v_refract};
            transparencyRay.point = hitPoint.point;
            auto nextRI = material.refractiveIndex;
            if (refractiveIndex != 1) {
                // if we're already in a non-ri-of-one material, go to 1
                nextRI = 1;
            }
            hitColour += (1 - material.opacity) * computeColour(transparencyRay, rayDepth - 1, nextRI);
        }
    }
    // reflection
    if (!reflectivity.is_zero()) {
        Vector n = hitPoint.normal;
        Vector v = ray.direction;

        v /= v.norm();
        n /= n.norm();

        auto v_prime = v / std::abs(n.dot(v));
        auto r = v_prime + 2 * n;
        r /= r.norm();

        Ray mirrorRay;
        // the mirror ray is reflected
        mirrorRay.direction = Direction{r};
        mirrorRay.point = hitPoint.point;
        // use the same refractive index as the medium stays the same
        hitColour += reflectivity * computeColour(mirrorRay, rayDepth - 1, refractiveIndex);
    }

    return hitColour;
}

bool Scene::hasCamera() const {
    return bool(camera_);
}
