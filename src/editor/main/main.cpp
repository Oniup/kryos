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

#include "core/console.h"
#include "core/input.h"
#include "vulkan_rhi/context.h"
#include "vulkan_rhi/window_handle.h"
#include <fmt/format.h>

int main()
{
#ifndef NDEBUG
    ky::ConsoleManager console(
        {
            new ky::ConsoleTerminalOutput(ky::CONSOLE_FLUSH_PER_MSG_BIT | ky::CONSOLE_COLOR_BIT |
                                          ky::CONSOLE_BREAK_AFTER_INFO),
        },
        ky::ConsoleManager::VERBOSE_MESSAGING);
#else
    ky::ConsoleManager console(
        {new ky::ConsoleTerminalOutput(ky::CONSOLE_FLUSH_PER_MSG_BIT | ky::CONSOLE_COLOR_BIT |
                                       ky::CONSOLE_BREAK_AFTER_INFO),
         });
#endif

    ky::WindowHandle window;
    ky::Input input;

    ky::VulkanContext vulkan(&window, "Kryos Engine", 0, 0,
                             ky::WINDOW_HANDLE_WINDOWED_BIT | ky::WINDOW_HANDLE_RESIZEABLE_BIT |
                                 ky::WINDOW_HANDLE_VSYNC_BIT);

    ky::Input::init(input, window);

    // while (!window.closing()) {
    //     input.poll_events();
    // }

    window.shutdown();
    vulkan.shutdown();
    console.shutdown();
}
