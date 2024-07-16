# Kryos Engine

A small cross platform game engine written in C++ aiming to provided a streamlined development
experience while staying lightweight, fast and open source.

> __NOTE:__ This software is very early within development and currently working on the early core
> components and tools.

# Build

### Windows

* ___CMake:___
    + Download: https://cmake.org/download/
* ___Ninja:___
    + Download: https://github.com/ninja-build/ninja/releases

___One or more of the following compilers needs to be installed:___

* ___GCC (MinGW):___
    + Download: https://www.mingw-w64.org/downloads/
* ___Clang:___
    + Download: https://llvm.org/builds/
* ___MSVC (Visual Studio):___
    + Download: https://visualstudio.microsoft.com/downloads/

### Linux

Make sure the install all dependencies. Run the following command based on your distribution:

* ___Debian:___
  ```bash
  sudo apt install libwayland-dev libxkbcommon-dev xorg-dev cmake ninja-build
  ```
* ___Fedora:___
  ```bash
  sudo dnf install wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel
                   libXrandr-devel gtk3-devel cmake ninja
  ```
* ___Arch:___
  ```bash
  sudo pacman -S libglvnd libxkbcommon cmake extra-cmake-modules ninja libxcursor libxi libxinerama
                 libxrandr vulkan-headers vulkan-icd-loader wayland-protocols
  ```

# Contributing

All contributions are welcome! Please see the [Guidelines](./CONTRIBUTE.md) if your interested.

# License

Kryos Engine is licensed under the MIT License - see the [License](./LICENSE) file for details.
