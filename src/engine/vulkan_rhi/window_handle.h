// This file is part of Kryos Engine (https://github.com/Oniup/kryos-engine)
// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef KRYOS_CORE__WINDOW_HANDLE_H
#define KRYOS_CORE__WINDOW_HANDLE_H

#include <cstdlib>
#define KY_WINDOW_HANDLE_DEFAULT_OPTIONS \
    ky::WINDOW_HANDLE_RESIZEABLE_BIT | ky::WINDOW_HANDLE_VSYNC_BIT

#define KY_WINDOW_HANDLE_DEFAULT KY_WINDOW_HANDLE_DEFAULT_OPTIONS | ky::WINDOW_HANDLE_WINDOWED_BIT

#include "vulkan_rhi/context.h"
#include <glm/glm.hpp>
#include <string_view>

struct GLFWwindow;

namespace ky {

enum WindowHandleFlags {
    WINDOW_HANDLE_NONE_BIT = 0,
    WINDOW_HANDLE_WINDOWED_BIT = 1 << 0,
    WINDOW_HANDLE_BORDERLESS_BIT = 1 << 1,
    WINDOW_HANDLE_FULLSCREEN_BIT = 1 << 2,
    WINDOW_HANDLE_VSYNC_BIT = 1 << 3,
    WINDOW_HANDLE_RESIZEABLE_BIT = 1 << 4,
    WINDOW_HANDLE_TRANSPARENT_BUFFER_BIT = 1 << 5,
};

struct WindowHandle {
    int opts;
    GLFWwindow* handle;
    VulkanContext* cntx;
    VkSurfaceKHR surface;

    bool init(VulkanContext* rhi, const std::string_view& title, int width, int height,
              int opts = KY_WINDOW_HANDLE_DEFAULT);
    void shutdown();

    bool closing() const;
    inline bool is_valid() const { return handle != nullptr; }

    const std::string_view title() const;
    glm::ivec2 framebuffer_size() const;
    glm::ivec2 size() const;
    glm::ivec2 position() const;

    void close(bool close = true);
};

} // namespace ky

#endif
