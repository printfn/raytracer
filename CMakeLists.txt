cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Building in release mode by default significantly
# improves raytracing performance
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# enable extra warnings
add_compile_options(-Wall -Wextra)

# Debug flags: assuming gcc/clang
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# Release flags: optimise raytracing performance
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(raytracer)

add_executable(raytracer
    src/AmbientLightSource.cpp
    src/AmbientLightSource.h
    src/Camera.h
    CImg/CImg.h
    src/Colour.cpp
    src/Colour.h
    src/Cube.cpp
    src/Cube.h
    src/Cylinder.cpp
    src/Cylinder.h
    src/Direction.cpp
    src/Direction.h
    src/DirectionalLightSource.cpp
    src/DirectionalLightSource.h
    src/ImageDisplay.cpp
    src/ImageDisplay.h
    src/LightSource.cpp
    src/LightSource.h
    src/Material.h
    src/Matrix.cpp
    src/Matrix.h
    src/Mesh.cpp
    src/Mesh.h
    src/NonCopyable.h
    src/Normal.cpp
    src/Normal.h
    src/Object.h
    src/PinholeCamera.cpp
    src/PinholeCamera.h
    src/Plane.cpp
    src/Plane.h
    src/Point.cpp
    src/Point.h
    src/PointLightSource.cpp
    src/PointLightSource.h
    src/Ray.h
    src/RayIntersection.h
    src/RaytracerOptions.h
    src/Scene.cpp
    src/Scene.h
    src/SceneReader.cpp
    src/SceneReader.h
    src/Sphere.cpp
    src/Sphere.h
    src/SpotLightSource.cpp
    src/SpotLightSource.h
    src/Transform.cpp
    src/Transform.h
    src/Triangle.cpp
    src/Triangle.h
    src/Vector.cpp
    src/Vector.h
    src/main.cpp
    src/utility.h)

target_compile_definitions(raytracer PRIVATE cimg_use_png=1)
target_compile_definitions(raytracer PRIVATE cimg_display=1)
target_include_directories(raytracer PRIVATE CImg)

find_package(X11 REQUIRED)
message("Found X11 in ${X11_INCLUDE_DIR}")
target_include_directories(raytracer PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(raytracer PRIVATE ${X11_LIBRARIES})

find_package(PNG REQUIRED)
message("Found libpng: ${PNG_VERSION_STRING}")
target_link_libraries(raytracer PRIVATE ${PNG_LIBRARIES})

target_link_libraries(raytracer PRIVATE pthread)
