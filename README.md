# Raytracer

This code has been tested on Linux and macOS, and with both Clang and GCC.

The following dependencies are *required*:

* CMake (to build the project)
* libpng
* X11 (available on macOS via XQuartz)

CMake should be able to find libpng and X11 automatically.

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
--no-gui        Disable the X11 GUI
-o <file.png>   Change the output filename to <file.png>
-j <n>          Use <n> threads for rendering
```
