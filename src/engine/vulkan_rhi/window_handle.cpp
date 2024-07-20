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

#include "vulkan_rhi/window_handle.h"
#include "core/console.h"
#include <GLFW/glfw3.h>

namespace ky {

bool WindowHandle::init(VulkanContext* cntx, const std::string_view& title, int width, int height,
                        int opts)
{
    KY_VULKAN_CONDITION_ERROR_RETURN(title[title.size()] == '\0', false,
                                     "Window title string must be null terminated");
    KY_VULKAN_CONDITION_ERROR_RETURN(title[title.size()] == '\0', false,
                                     "Window title string must be null terminated");
    KY_VULKAN_CONDITION_ERROR_RETURN(
        !(opts & WINDOW_HANDLE_FULLSCREEN_BIT && opts & WINDOW_HANDLE_WINDOWED_BIT), false,
        "Cannot set window to fullscreen and windowed mode at the same time");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (width == 0 || height == 0) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width / 2;
        height = mode->height / 2;
    }
    if (opts & ~WINDOW_HANDLE_FULLSCREEN_BIT) {
        monitor = nullptr;
    }

    handle = glfwCreateWindow(width, height, title.data(), monitor, nullptr);
    KY_VULKAN_CONDITION_ERROR_RETURN(handle != nullptr, false, "Failed to create GLFW window");

    VkResult result = glfwCreateWindowSurface(cntx->_instance.instance, handle, nullptr, &surface);
    KY_VULKAN_CONDITION_ERROR_RETURN(result == VK_SUCCESS, false,
                                     "GLFW failed to create vulkan surface");

    this->opts = opts;
    this->cntx = cntx;
    return true;
}

void WindowHandle::shutdown()
{
    if (handle != nullptr) {
        vkDestroySurfaceKHR(cntx->_instance.instance, surface, nullptr);
        glfwDestroyWindow(handle);

        opts = WINDOW_HANDLE_NONE_BIT;
        handle = nullptr;
        cntx = nullptr;
        surface = nullptr;
    }
}

bool WindowHandle::closing() const
{
    return glfwWindowShouldClose(handle);
}

const std::string_view WindowHandle::title() const
{
    return std::string_view(glfwGetWindowTitle(handle));
}

glm::ivec2 WindowHandle::framebuffer_size() const
{
    glm::ivec2 size;
    glfwGetFramebufferSize(handle, &size.x, &size.y);
    return size;
}

glm::ivec2 WindowHandle::size() const
{
    glm::ivec2 size;
    glfwGetWindowSize(handle, &size.x, &size.y);
    return size;
}

glm::ivec2 WindowHandle::position() const
{
    glm::ivec2 pos;
    glfwGetWindowPos(handle, &pos.x, &pos.y);
    return pos;
}

void WindowHandle::close(bool close)
{
    glfwSetWindowShouldClose(handle, close);
}

} // namespace ky
