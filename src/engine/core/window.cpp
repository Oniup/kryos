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

#include "core/window.h"

#include "core/error.h"
#include "core/macros.h"

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ky {

void WindowHandle::init(const std::string_view& title, int width, int height, WindowHandle* parent,
                        int opts) {
    KY_FATAL_CONDITION_MSG(title[title.size()] == '\0', "Title string must be null terminated");
    KY_FATAL_CONDITION_MSG(
        !(opts & WINDOW_HANDLE_FULLSCREEN_BIT && opts & WINDOW_HANDLE_WINDOWED_BIT),
        "Cannot set fullscreen and windowed mode at the same time");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // glfwWindowHint(GLFW_RESIZABLE, (opts & WINDOW_HANDLE_RESIZEABLE_BIT) != 0);
    // glfwWindowHint(GLFW_DECORATED, (opts & ~WINDOW_HANDLE_BORDERLESS_BIT) != 0);
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
    //                (opts & WINDOW_HANDLE_TRANSPARENT_BUFFER_BIT) != 0);

    // #ifdef __APPLE__
    //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // #endif

    GLFWwindow* share = nullptr;
    if (parent != nullptr) {
        share = parent->glfw_handle;
        this->parent = parent;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (width == 0 || height == 0) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width / 2;
        height = mode->height / 2;
    }

    if (opts & ~WINDOW_HANDLE_FULLSCREEN_BIT) {
        monitor = nullptr;
    }

    glfw_handle = glfwCreateWindow(width, height, title.data(), monitor, share);
    KY_FATAL_CONDITION_MSG(glfw_handle != nullptr, "Failed to create GLFW window");

    // if (opts & WINDOW_HANDLE_VSYNC_BIT) {
    //     glfwSwapInterval(0);
    // } else {
    //     glfwSwapInterval(1);
    // }

    options = opts;
}

void WindowHandle::shutdown(bool remove_child_ref_from_parent) {
    KY_ERROR_CONDITION_MSG(glfw_handle != nullptr, "WindowHandle is not valid");

    for (WindowHandle& child : children) {
        child.shutdown(false);
    }
    std::string window_title(title());
    glfwDestroyWindow(glfw_handle);

    if (remove_child_ref_from_parent && parent != nullptr) {
        size_t i = 0;
        for (; i < parent->children.size(); i++) {
            if (parent->children[i] == *this) {
                break;
            }
        }
        if (i < parent->children.size()) {
            parent->children.erase(parent->children.begin() + i);
        } else {
            KY_ERROR_MSG("Could not find '%.*s' window in parent window", KY_STR(window_title));
        }
    }
    glfw_handle = nullptr;
    options = WINDOW_HANDLE_NONE_BIT;
    parent = nullptr;
    children.clear();
}

WindowHandle& WindowHandle::create_window(const std::string_view& title, int width, int height,
                                          int opts) {
    WindowHandle child;
    child.init(title, width, height, this, opts);
    children.push_back(std::move(child));
    return children.back();
}

bool WindowHandle::closing() const {
    return glfwWindowShouldClose(glfw_handle);
}

const std::string_view WindowHandle::title() const {
    return std::string_view(glfwGetWindowTitle(glfw_handle));
}

glm::ivec2 WindowHandle::framebuffer_size() const {
    glm::ivec2 buffer_size;
    glfwGetFramebufferSize(glfw_handle, &buffer_size.x, &buffer_size.y);
    return buffer_size;
}

glm::ivec2 WindowHandle::size() const {
    glm::ivec2 win_size;
    glfwGetWindowSize(glfw_handle, &win_size.x, &win_size.y);
    return win_size;
}

glm::ivec2 WindowHandle::position() const {
    glm::ivec2 win_pos;
    glfwGetWindowPos(glfw_handle, &win_pos.x, &win_pos.y);
    return win_pos;
}

void WindowHandle::close(bool close) {
    glfwSetWindowShouldClose(glfw_handle, close);
}

WindowManager::WindowManager(const std::string_view& title, int opts)
      : WindowManager(title, 0, 0, opts) {
}

WindowManager::WindowManager(const std::string_view& title, int width, int height, int opts) {
    KY_FATAL_CONDITION_MSG(glfwInit(), "Failed to initialize GLFW");
    glfwSetErrorCallback(_window_handle_error_callback);

    _main.init(title, width, height, nullptr, opts);
}

void WindowManager::shutdown() {
    _main.shutdown();
    glfwTerminate();
}

bool WindowManager::continue_runtime_loop() {
    _remove_closed_windows(_main);
    return _main.valid() && !_main.closing();
}

void WindowManager::swap_buffers() {
    _swap_buffers(_main);
}

void WindowManager::_window_handle_error_callback(int error, const char* description) {
    KY_ERROR_MSG("GLFW Error %d: %s", error, description);
}

void WindowManager::_remove_closed_windows(WindowHandle& handle) {
    if (handle.closing()) {
        handle.shutdown();
    } else {
        for (WindowHandle& child : handle.children) {
            _remove_closed_windows(child);
        }
    }
}

void WindowManager::_swap_buffers(WindowHandle& handle) {
    // Swap buffer logic...
    for (WindowHandle& child : handle.children) {
        _swap_buffers(child);
    }
}

} // namespace ky
