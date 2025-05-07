cmake_minimum_required(VERSION 3.22) # compatible with Ubuntu 22.04

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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
	src/RaytracerOptions.cpp
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

set(USE_GUI "AUTO" CACHE STRING "Enable GUI support [AUTO (default), SDL3, X11, OFF]")
if(USE_GUI STREQUAL "AUTO")
	message(STATUS "Auto-detecting GUI")
	find_package(SDL3)
	if(NOT SDL3_FOUND)
		find_package(X11)
	endif()
elseif(USE_GUI STREQUAL "SDL3")
	message(STATUS "SDL3 is required")
	find_package(SDL3 REQUIRED)
elseif(USE_GUI STREQUAL "X11")
	message(STATUS "X11 is required")
	find_package(X11 REQUIRED)
elseif(USE_GUI STREQUAL "OFF" OR USE_GUI STREQUAL "NO")
	message(STATUS "GUI support is disabled")
else()
	message(FATAL_ERROR "USE_GUI is not set correctly: must be AUTO (default), SDL3, X11 or OFF (currently set to '" ${USE_GUI} "')")
endif()

if(SDL3_FOUND)
	message(STATUS "Found SDL3: ${SDL3_LIBRARIES}")
	target_include_directories(raytracer PRIVATE ${SDL3_INCLUDE_DIR})
	target_link_libraries(raytracer PRIVATE ${SDL3_LIBRARIES})
	target_compile_definitions(raytracer PRIVATE USE_GUI=1)

	target_compile_definitions(raytracer PRIVATE cimg_display=3)
	target_include_directories(raytracer PRIVATE CImg)
elseif(X11_FOUND)
	message(STATUS "Found X11: ${X11_LIBRARIES}")
	target_include_directories(raytracer PRIVATE ${X11_INCLUDE_DIR})
	target_link_libraries(raytracer PRIVATE ${X11_LIBRARIES})
	target_compile_definitions(raytracer PRIVATE USE_GUI=1)

	target_compile_definitions(raytracer PRIVATE cimg_display=1)
	target_include_directories(raytracer PRIVATE CImg)
else()
	target_compile_definitions(raytracer PRIVATE USE_GUI=0)
endif()

find_package(JPEGXL REQUIRED)
message(STATUS "Found libjxl: ${JPEGXL_LIBRARIES}")
target_include_directories(raytracer PRIVATE ${JPEGXL_INCLUDE_DIRS})
target_link_libraries(raytracer PRIVATE ${JPEGXL_LIBRARIES})

target_link_libraries(raytracer PRIVATE pthread)
