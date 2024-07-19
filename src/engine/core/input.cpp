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

#define GLFW_INCLUDE_VULKAN

#include "core/input.h"
#include "core/console.h"
#include <GLFW/glfw3.h>

namespace ky {

Input* Input::_instance = nullptr;

#define KY_REG_ONCE_IMPL(func, input_type, code, pressed)              \
    bool result = func(code);                                          \
    if (result) {                                                      \
        result = _instance->_register_once(input_type, code, pressed); \
    }                                                                  \
    return result

#define KY_REG_ONCE_WITH_HANDLE_IMPL(func, handle, input_type, code, pressed) \
    bool result = func(handle, code);                                         \
    if (result) {                                                             \
        result = _instance->_register_once(input_type, code, pressed);        \
    }                                                                         \
    return result

void Input::init(Input& instance, WindowManager& window_manager)
{
    _instance = &instance;
    _instance->_window_manager = &window_manager;
    for (_Registered& reg : _instance->_reg_once_buffer) {
        reg.type = InputType::UNKNOWN;
    }
}

bool Input::key_pressed(KeyCode code)
{
    KY_REG_ONCE_IMPL(key_press, InputType::KEYBOARD, code, true);
}

bool Input::key_released(KeyCode code)
{
    KY_REG_ONCE_IMPL(key_release, InputType::KEYBOARD, code, false);
}

bool Input::key_pressed(const WindowHandle& handle, KeyCode code)
{
    KY_REG_ONCE_WITH_HANDLE_IMPL(key_press, handle, InputType::KEYBOARD, code, true);
}

bool Input::key_released(const WindowHandle& handle, KeyCode code)
{
    KY_REG_ONCE_WITH_HANDLE_IMPL(key_release, handle, InputType::KEYBOARD, code, false);
}

bool Input::key_press(KeyCode code)
{
    return glfwGetKey(_instance->_window_manager->main().glfw_handle, code) == GLFW_PRESS;
}

bool Input::key_release(KeyCode code)
{
    return glfwGetKey(_instance->_window_manager->main().glfw_handle, code) == GLFW_RELEASE;
}

bool Input::key_press(const WindowHandle& handle, KeyCode code)
{
    return glfwGetKey(handle.glfw_handle, code) == GLFW_PRESS;
}

bool Input::key_release(const WindowHandle& handle, KeyCode code)
{
    return glfwGetKey(handle.glfw_handle, code) == GLFW_RELEASE;
}

bool Input::mouse_pressed(MouseButton button)
{
    KY_REG_ONCE_IMPL(mouse_press, InputType::MOUSE, button, true);
}

bool Input::mouse_released(MouseButton button)
{
    KY_REG_ONCE_IMPL(mouse_release, InputType::MOUSE, button, false);
}

bool Input::mouse_pressed(const WindowHandle& handle, MouseButton button)
{
    KY_REG_ONCE_WITH_HANDLE_IMPL(mouse_press, handle, InputType::MOUSE, button, true);
}

bool Input::mouse_released(const WindowHandle& handle, MouseButton button)
{
    KY_REG_ONCE_WITH_HANDLE_IMPL(mouse_release, handle, InputType::MOUSE, button, false);
}

bool Input::mouse_press(MouseButton button)
{
    return glfwGetMouseButton(_instance->_window_manager->main().glfw_handle, button) ==
           GLFW_PRESS;
}

bool Input::mouse_release(MouseButton button)
{
    return glfwGetMouseButton(_instance->_window_manager->main().glfw_handle, button) ==
           GLFW_RELEASE;
}

bool Input::mouse_press(const WindowHandle& handle, MouseButton button)
{
    return glfwGetMouseButton(handle.glfw_handle, button) == GLFW_PRESS;
}

bool Input::mouse_release(const WindowHandle& handle, MouseButton button)
{
    return glfwGetMouseButton(handle.glfw_handle, button) == GLFW_RELEASE;
}

std::string_view Input::type_to_string(InputType type)
{
    switch (type) {
        case InputType::KEYBOARD:
            return std::string_view("Keyboard");
        case InputType::MOUSE:
            return std::string_view("Mouse");
        case InputType::GAME_PAD:
            return std::string_view("Game Pad");
        default:
            return std::string_view("Unknown");
    }
}

std::string_view Input::mouse_mode_to_string(MouseMode mode)
{
    switch (mode) {
        case MouseMode::VISABLE:
            return std::string_view("Visable");
        case MouseMode::HIDDEN:
            return std::string_view("Hidden");
        case MouseMode::CAPTURED:
            return std::string_view("Captured");
        default:
            return std::string_view("Invalid");
    }
}

void Input::poll_events()
{
    glfwPollEvents();
    size_t reg_counted = 0;
    for (_Registered& reg : _instance->_reg_once_buffer) {
        if (reg_counted == _instance->_reg_count) {
            break;
        } else if (reg.type == InputType::UNKNOWN) {
            continue;
        }
        reg_counted++;

        if (!reg.remove_next_frame) {
            reg.remove_next_frame = true;
        } else {
            reg.type = InputType::UNKNOWN;
            _instance->_reg_count--;
        }
    }
}

bool Input::_register_once(InputType type, int code, bool pressed)
{
    int free_index = -1;
    for (size_t i = 0; i < _reg_once_buffer.size(); i++) {
        if (_reg_once_buffer[i].type == InputType::UNKNOWN) {
            free_index = i;
            continue;
        }
        bool found = _reg_once_buffer[i].type == type && _reg_once_buffer[i].code == code &&
                     _reg_once_buffer[i].pressed == pressed;
        if (found) {
            _reg_once_buffer[i].remove_next_frame = false;
            return false;
        }
    }
    KY_CONTEXT_CONDITION_ERROR_RETURN("INPUT", free_index != -1, false,
                                      "Toggle register buffer full, cannot add {} {}",
                                      type_to_string(type), code);

    _reg_once_buffer[free_index] = _Registered {
        .type = type,
        .code = code,
        .remove_next_frame = false,
        .pressed = pressed,
    };
    _reg_count++;
    return true;
}

} // namespace ky
