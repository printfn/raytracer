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
    AmbientLightSource.cpp
    AmbientLightSource.h
    Camera.h
    CImg/CImg.h
    Colour.cpp
    Colour.h
    Cube.cpp
    Cube.h
    Cylinder.cpp
    Cylinder.h
    Direction.cpp
    Direction.h
    DirectionalLightSource.cpp
    DirectionalLightSource.h
    ImageDisplay.cpp
    ImageDisplay.h
    LightSource.cpp
    LightSource.h
    Material.h
    Matrix.cpp
    Matrix.h
    Mesh.cpp
    Mesh.h
    NonCopyable.h
    Normal.cpp
    Normal.h
    Object.h
    PinholeCamera.cpp
    PinholeCamera.h
    Plane.cpp
    Plane.h
    Point.cpp
    Point.h
    PointLightSource.cpp
    PointLightSource.h
    Ray.h
    RayIntersection.h
    RaytracerOptions.h
    Scene.cpp
    Scene.h
    SceneReader.cpp
    SceneReader.h
    Sphere.cpp
    Sphere.h
    SpotLightSource.cpp
    SpotLightSource.h
    Transform.cpp
    Transform.h
    Triangle.cpp
    Triangle.h
    Vector.cpp
    Vector.h
    main.cpp
    utility.h)

target_compile_definitions(raytracer PRIVATE cimg_use_png=1)
target_compile_definitions(raytracer PRIVATE cimg_display=1)

find_package(X11 REQUIRED)
message("Found X11 in ${X11_INCLUDE_DIR}")
target_include_directories(raytracer PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(raytracer PRIVATE ${X11_LIBRARIES})

find_package(PNG REQUIRED)
message("Found libpng: ${PNG_VERSION_STRING}")
target_link_libraries(raytracer PRIVATE ${PNG_LIBRARIES})

target_link_libraries(raytracer PRIVATE pthread)
