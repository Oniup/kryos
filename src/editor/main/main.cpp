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
#include "core/window.h"
#include "renderer_hardware/vulkan_rhi.h"
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
    ky::ConsoleManager console({
        new ky::ConsoleTerminalOutput(ky::CONSOLE_FLUSH_PER_MSG_BIT | ky::CONSOLE_COLOR_BIT |
                                      ky::CONSOLE_BREAK_AFTER_INFO),
    });
#endif

    ky::WindowManager windows("Kryos Engine");

    ky::VulkanInstance::print_instance_capabilities();
#ifndef NDEBUG
    ky::VulkanInstance::print_validation_capabilities();
#endif
    ky::RenderHardware rhi("Kryos Engine");

    ky::Input input;
    ky::Input::init(input, windows);

    // while (window_manager.continue_runtime_loop()) {
    //     window_manager.swap_buffers();
    //     input.poll_events();
    // }

    rhi.shutdown();
    windows.shutdown();
    console.shutdown();
}
