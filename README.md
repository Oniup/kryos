# Kryos Engine

A small cross platform game engine written in C++ aiming to provided a streamlined development
experience while staying lightweight, fast and open source.

> **NOTE:** This software is very early within development and currently working on the early core
> components and tools.

# Build

### Windows

* ***CMake:***
    + Download: https://cmake.org/download/
* ***Ninja:***
    + Download: https://github.com/ninja-build/ninja/releases

***One or more of the following compilers needs to be installed:***

* ***GCC (MinGW):***
    + Download: https://www.mingw-w64.org/downloads/
* ***Clang:***
    + Download: https://llvm.org/builds/
* ***MSVC (Visual Studio):***
    + Download: https://visualstudio.microsoft.com/downloads/

### Linux

Make sure the install all dependencies. Run the following command based on your distribution:

* ***Debian:***
    ```bash
    sudo apt install libwayland-dev libxkbcommon-dev xorg-dev cmake ninja-build
    ```
* ***Fedora:***
    ```bash
    sudo dnf install wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel \
                     libXinerama-devel libXrandr-devel gtk3-devel cmake ninja
    ```
* ***Arch:***
    ```bash
    sudo pacman -S libglvnd libxkbcommon cmake extra-cmake-modules ninja libxcursor libxi \
                   libxinerama libxrandr vulkan-headers vulkan-icd-loader vulkan-tools \
                   wayland-protocols
    ```
    + for debug build, if you want Vulkan's validation layers enabled, install 
      `vulkan-validation-layers`

# Contributing

All contributions are welcome! Please see the [Guidelines](./CONTRIBUTE.md) if your interested.

# License

Kryos Engine is licensed under the MIT License - see the [License](./LICENSE) file for details.
