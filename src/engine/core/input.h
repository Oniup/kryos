// This file iGLFW_INCLUDE_NONEs part of Kryos Engine (https://github.com/Oniup/kryos-engine)
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

#ifndef KRYOS_CORE__INPUT_H
#define KRYOS_CORE__INPUT_H

#include "core/input_keycodes.h"
#include "core/window.h"

#include <array>

namespace ky {

enum InputType {
    INPUT_TYPE_UNKNOWN = -1,
    INPYT_TYPE_KEYBOARD,
    INPUT_TYPE_MOUSE,
    INPUT_TYPE_GAME_PAD,
};

enum MouseMode {
    MOUSE_MODE_VISABLE,
    MOUSE_MODE_HIDDEN,
    MOUSE_MODE_CAPTURED,
};

class Input {
    struct _Registered {
        InputType type = INPUT_TYPE_UNKNOWN;
        int code = -1;
        bool remove_next_frame = false;
        bool pressed = false;
    };

public:
    static constexpr size_t REG_ONCE_BUFFER_SIZE = 16;

    static void init(Input& instance, WindowManager& window_manager);

    static bool key_pressed(KeyCode code);
    static bool key_released(KeyCode code);
    static bool key_pressed(const WindowHandle& handle, KeyCode code);
    static bool key_released(const WindowHandle& handle, KeyCode code);

    static bool key_press(KeyCode code);
    static bool key_release(KeyCode code);
    static bool key_press(const WindowHandle& handle, KeyCode code);
    static bool key_release(const WindowHandle& handle, KeyCode code);

    static bool mouse_pressed(MouseButton button);
    static bool mouse_released(MouseButton button);
    static bool mouse_pressed(const WindowHandle& handle, MouseButton button);
    static bool mouse_released(const WindowHandle& handle, MouseButton button);

    static bool mouse_press(MouseButton button);
    static bool mouse_release(MouseButton button);
    static bool mouse_press(const WindowHandle& handle, MouseButton button);
    static bool mouse_release(const WindowHandle& handle, MouseButton button);

    void poll_events();

private:
    WindowManager* _window_manager = nullptr;
    size_t _reg_count = 0;
    std::array<_Registered, REG_ONCE_BUFFER_SIZE> _reg_once_buffer;

    static Input* _instance;

    bool _register_once(InputType type, int code, bool pressed);
};

} // namespace ky

#endif
