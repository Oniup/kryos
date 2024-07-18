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
#include "renderer_hardware/vulkan_capabilities.h"
#include "renderer_hardware/vulkan_context.h"
#include <fmt/format.h>

void print_rhi_info()
{
#ifndef NDEBUG
    bool validation_layers_enabled = true;
    fmt::println("\nAvailable vulkan validation layers:");
    for (VkLayerProperties& property : ky::VulkanCapabilities::available_validation_layers()) {
        fmt::println(" - {}", property.layerName);
    }

    fmt::println("\nRequired vulkan validation layers:");
    for (const char* name : ky::VulkanCapabilities::required_validation_layers()) {
        fmt::println(" - {}", name);
    }
#else
    bool validation_layers_enabled = false;
#endif

    fmt::println("\nAvailable vulkan instance extensions:");
    for (VkExtensionProperties& property :
         ky::VulkanCapabilities::available_instance_extensions()) {
        fmt::println(" - {}", property.extensionName);
    }

    fmt::println("\nRequired vulkan instance extensions:");
    for (const char* name :
         ky::VulkanCapabilities::required_instance_extensions(validation_layers_enabled)) {
        fmt::println(" - {}", name);
    }
}

void test_console_macros()
{
    KY_CONTEXT_VERBOSE("HEADING", "This is {}", "a test");
    KY_CONTEXT_INFO("HEADING", "This is {}", "a test");
    KY_CONTEXT_WARN("HEADING", "This is {}", "a test");
    KY_CONTEXT_ERROR("HEADING", "This is {}", "a test");
    KY_VULKAN_CONDITION_FATAL(false, "This is {}", "a test");
}

int main()
{
    ky::ConsoleManager console({
        new ky::ConsoleTerminalOutput(ky::CONSOLE_FLUSH_PER_MSG_BIT | ky::CONSOLE_COLOR_BIT |
                                      ky::CONSOLE_BREAK_AFTER_INFO),
    });

    test_console_macros();

    ky::WindowManager windows("Kryos Engine");

    print_rhi_info();
    ky::VulkanContext render_hardware("Kryos Engine", windows);

    ky::Input input;
    ky::Input::init(input, windows);

    // while (window_manager.continue_runtime_loop()) {
    //     window_manager.swap_buffers();
    //     input.poll_events();
    // }

    render_hardware.shutdown();
    windows.shutdown();
    console.shutdown();
}
