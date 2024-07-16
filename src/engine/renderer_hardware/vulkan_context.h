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

#include <array>
#include <string_view>
#include <vulkan/vulkan.h>

namespace ky {

class VulkanContext {
    friend WindowManager;

public:
    static constexpr std::array<const char*, 2> CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES {
        "Extension",
        "Another extension",
    };

    static std::vector<const char*> required_instance_extensions_names();
    static std::vector<std::string> available_instance_extensions_names();

    VulkanContext(const std::string_view& app_name, WindowManager& window_manager);

    void shutdown();

private:
    WindowManager* _window_manager = nullptr;
    VkApplicationInfo _app_info;
    VkInstance _instance = nullptr;

    static std::vector<VkExtensionProperties> _available_instance_extensions_properties();

    bool _init_instance();
};

} // namespace ky

#endif
