cmake_minimum_required(VERSION 3.20)

# CMake Policies
# ------------------------------------------------------------------------------
cmake_policy(SET CMP0003 NEW) # Libraries linked via full path != linker search
# path error.
cmake_policy(SET CMP0010 NEW) # Syntax problems are errors.

# CMake Project
# ------------------------------------------------------------------------------
project(kryos_engine)
include(build_files/compiler.cmake)
set(VERSION_BUILD_INFO
    "0.0.1"
    CACHE INTERNAL "")

# Kryos build options
# ------------------------------------------------------------------------------
option(BUILD_CORE_LIB "Core static library for the editor/games runtimes" ON)
option(BUILD_EDITOR_EXE "Build kryos editor executable" ON)
option(BUILD_TESTS_EXE "Build all kryos library tests" ON)

# Build directories
# ------------------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project source files
# ------------------------------------------------------------------------------
if (${BUILD_CORE_LIB})
    add_subdirectory(src)
endif()
