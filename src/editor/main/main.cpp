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

#include "core/error.h"
#include "core/input.h"
#include "core/window.h"
#include "renderer_hardware/vulkan_context.h"

#include <fmt/format.h>

void print_rhi_info() {
    fmt::println("Available vulkan instance extensions:");
    for (std::string& name : ky::VulkanContext::available_instance_extensions_names()) {
        fmt::println(" - {}", name);
    }

    fmt::println("\nRequired instance extension:");
    for (const char* name : ky::VulkanContext::required_instance_extensions_names()) {
        fmt::println(" - {}", name);
    }
}

int main(int argc, char** argv) {
    std::vector<const char*> cmd_line_args(argc);
    for (int i = 0; i < argc; i++) {
        cmd_line_args[i] = argv[i];
    }

    ky::error::init();
    ky::WindowManager window_manager("Kryos Engine");

    ky::VulkanContext rhi_context("Kryos Engine", window_manager);
    print_rhi_info();

    ky::Input input;
    ky::Input::init(input, window_manager);

    // // Test windows
    // ky::WindowHandle& child = window_manager.create_window("Test window", 500, 500);
    // child.create_window("Child of test window", 400, 400,
    //                     ky::WINDOW_HANDLE_WINDOWED_BIT | ky::WINDOW_HANDLE_VSYNC_BIT);

    // while (window_manager.continue_runtime_loop()) {
    //     window_manager.swap_buffers();
    //     input.poll_events();
    // }

    rhi_context.shutdown();
    window_manager.shutdown();
    ky::error::shutdown();
}
