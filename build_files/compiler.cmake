# This file is part of Kryos Engine (https://github.com/Oniup/kryos-engine)
# Copyright (c) 2024 Oniup (https://github.com/Oniup)
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

# Default compiler options
# ------------------------------------------------------------------------------
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DEFAULT_COMPILE_DEFINITIONS DEBUG)
    # Debugging flags
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        set(DEFAULT_COMPILE_OPTIONS /EHsc /MTd /W2)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        set(DEFAULT_COMPILE_OPTIONS -Wall -Wextra -fdiagnostics-color=always)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(DEFAULT_COMPILE_OPTIONS -Wall -Wextra -fcolor-diagnostics)
    endif()
else()
    set(DEFAULT_COMPILE_DEFINITIONS NDEBUG)
    # Optimization flags
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        set(DEFAULT_COMPILE_OPTIONS /O2 /fp:fast)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        set(DEFAULT_COMPILE_OPTIONS -O2 -fdiagnostics-color=always)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(DEFAULT_COMPILE_OPTIONS -O2 -fcolor-diagnostics)
    endif()
endif()

# Default compiler options
# ------------------------------------------------------------------------------
if(WIN32)
    list(APPEND DEFAULT_COMPILE_DEFINITIONS KY_PLATFORM_WINDOWS)
elseif(UNIX)
    list(APPEND DEFAULT_COMPILE_DEFINITIONS KY_PLATFORM_UNIX)
    if(APPLE)
        list(APPEND DEFAULT_COMPILE_DEFINITIONS KY_PLATFORM_APPLE)
    else()
        list(APPEND DEFAULT_COMPILE_DEFINITIONS KY_PLATFORM_LINUX)
    endif()
endif()
