# This file is part of kryos Engine (https://github.com/Oniup/kryos-engine)
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

# Options
# -------------------------------------------------------------------------------
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)

# Sources
# -------------------------------------------------------------------------------
# Graphics
find_package(Vulkan REQUIRED)

# add_subdirectory(glad)
add_subdirectory(glfw)

# Linking to Kryos
# -------------------------------------------------------------------------------
target_include_directories(
    kryos
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/
            ${CMAKE_CURRENT_SOURCE_DIR}/glm
            ${CMAKE_CURRENT_SOURCE_DIR}/fmt
            ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
            # ${glfw_INCLUDE_DIRS}
            # ${glad_INCLUDE_DIRS}
            ${CMAKE_CURRENT_SOURCE_DIR}/entt/src
            ${CMAKE_CURRENT_SOURCE_DIR}/imgui
            ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(
    kryos
    PRIVATE glfw ${Vulkan_LIBRARIES}
)
