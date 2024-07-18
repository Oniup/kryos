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

#include "renderer_hardware/vulkan_capabilities.h"

#include <GLFW/glfw3.h>
#include <cstring>

namespace ky {

static std::vector<const char*> _CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES {};

#ifndef NDEBUG
static std::vector<const char*> _REQUIRED_VALIDATION_LAYER_NAMES {
    "VK_LAYER_KHRONOS_validation",
};
#endif

#ifndef NDEBUG
bool VulkanAttributes::check_required_validation_layers(const std::vector<const char*>& required) {
    std::vector<VkLayerProperties> properties = available_validation_layers();

    for (const char* layer : required) {
        bool found = false;
        for (VkLayerProperties& property : properties) {
            bool compare =
                std::strncmp(layer, property.layerName, std::strlen(property.layerName)) == 0;
            if (compare) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}
#endif

bool VulkanAttributes::check_required_instance_extensions(
    const std::vector<const char*>& required) {
    std::vector<VkExtensionProperties> properties = available_instance_extensions();

    for (const char* layer : required) {
        bool found = false;
        for (VkExtensionProperties& property : properties) {
            bool compare = std::strncmp(layer, property.extensionName,
                                        std::strlen(property.extensionName)) == 0;
            if (compare) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

#ifndef NDEBUG
std::vector<const char*> VulkanAttributes::required_validation_layers() {
    return _REQUIRED_VALIDATION_LAYER_NAMES;
}
#endif

std::vector<const char*> VulkanAttributes::required_instance_extensions(bool validation_layers) {
    uint32_t glfw_extension_count = 0;
    const char** glfw_extension_names = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extension_names,
                                        glfw_extension_names + glfw_extension_count);

    for (const char* cross_platform : _CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES) {
        extensions.push_back(cross_platform);
    }
    if (validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

#ifndef NDEBUG
std::vector<VkLayerProperties> VulkanAttributes::available_validation_layers() {
    uint32_t count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> properties(count);
    vkEnumerateInstanceLayerProperties(&count, properties.data());
    return properties;
}
#endif

std::vector<VkExtensionProperties> VulkanAttributes::available_instance_extensions() {
    unsigned int count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> properties(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());
#ifdef __APPLE__
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
    return properties;
}

} // namespace ky
