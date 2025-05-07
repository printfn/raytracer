# Raytracer

This code has been tested on Linux and macOS, and with both Clang and GCC.

The following dependencies are **required**:

* CMake
* libjxl

This raytracer also has optional GUI support using SDL3 or X11. By default it
will attempt to find either SDL3 or X11 on your system. You can customise this
behaviour by setting the `USE_GUI` parameter to `AUTO` (default), `SDL3`, `X11`
or `OFF` (e.g. `cmake -DUSE_GUI=OFF`).

Instructions for building:

```bash
mkdir build
cd build
cmake ..
make
./raytracer ../scenes/snowman.txt
```

The `raytracer` executable accepts the following command-line options:

```
--no-gui        Disable the GUI
-o <file.jxl>   Change the output filename to <file.jxl>
-j <n>          Use <n> threads for rendering
```
