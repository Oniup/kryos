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

#ifndef KRYOS_RENDERER_HARDWARE__VULKAN_CONTEXT_H
#define KRYOS_RENDERER_HARDWARE__VULKAN_CONTEXT_H

#include "core/window.h"
#include <string_view>
#include <vulkan/vulkan.h>

namespace ky {

class VulkanContext
{
    friend WindowManager;

public:
    VulkanContext(const std::string_view& app_name, WindowManager& window_manager);

    void shutdown();

private:
    WindowManager* _window_manager = nullptr;
    VkInstance _instance = nullptr;
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT _debug_messenger;
#endif

#ifndef NDEBUG
    bool _init_instance(const std::string_view& app_name, bool& validation_layers_enabled);
    bool _init_validation_layers();

    VkDebugUtilsMessengerCreateInfoEXT _debug_messenger_create_info();
#else
    bool _init_instance(const std::string_view& app_name);
#endif
};

} // namespace ky

#endif
