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

#ifndef KRYOS_CORE__WINDOW_H
#define KRYOS_CORE__WINDOW_H

#include <glm/glm.hpp>
#include <string_view>
#include <vector>

struct GLFWwindow;

#define KY_WINDOW_HANDLE_DEFAULT_OPTIONS \
    ky::WINDOW_HANDLE_RESIZEABLE_BIT | ky::WINDOW_HANDLE_VSYNC_BIT

#define KY_WINDOW_HANDLE_DEFAULT KY_WINDOW_HANDLE_DEFAULT_OPTIONS | ky::WINDOW_HANDLE_WINDOWED_BIT

// FIXME: Sometimes segfaults when testing with toggle input reg stuff

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
    GLFWwindow* glfw_handle = nullptr;
    int options = WINDOW_HANDLE_NONE_BIT;
    WindowHandle* parent = nullptr;
    std::vector<WindowHandle> children = {};

    void init(const std::string_view& title, int width, int height, WindowHandle* parent,
              int opts = KY_WINDOW_HANDLE_DEFAULT);

    void shutdown(bool remove_child_ref_from_parent = true);

    inline bool operator==(const WindowHandle& window) const {
        return glfw_handle == window.glfw_handle;
    }

    inline bool operator!=(const WindowHandle& window) const {
        return glfw_handle != window.glfw_handle;
    }

    WindowHandle& create_window(const std::string_view& title, int width, int height,
                                int opts = KY_WINDOW_HANDLE_DEFAULT);

    bool closing() const;
    inline bool valid() const { return glfw_handle != nullptr; }

    const std::string_view title() const;
    glm::ivec2 framebuffer_size() const;
    glm::ivec2 size() const;
    glm::ivec2 position() const;

    void close(bool close = true);
};

class WindowManager {
public:
    WindowManager(const std::string_view& title, int opts = KY_WINDOW_HANDLE_DEFAULT);
    WindowManager(const std::string_view& title, int width, int height,
                  int opts = KY_WINDOW_HANDLE_DEFAULT);
    ~WindowManager();

    inline WindowHandle& main() { return _main; }
    inline const WindowHandle& main() const { return _main; }

    inline WindowHandle& create_window(const std::string_view& title,
                                       int opts = KY_WINDOW_HANDLE_DEFAULT) {
        return create_window(title, 0, 0, opts);
    }

    inline WindowHandle& create_window(const std::string_view& title, int width, int height,
                                       int opts = KY_WINDOW_HANDLE_DEFAULT) {
        return _main.create_window(title, width, height, opts);
    }

    bool continue_runtime_loop();
    void swap_buffers();

private:
    WindowHandle _main;

    static void _window_handle_error_callback(int error, const char* description);
    static void _remove_closed_windows(WindowHandle& handle);
    static void _swap_buffers(WindowHandle& handle);
};

} // namespace ky

#endif
