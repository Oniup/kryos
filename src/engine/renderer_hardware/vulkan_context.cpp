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

#include "renderer_hardware/vulkan_context.h"

#include "core/error.h"

#include <GLFW/glfw3.h>
#include <fmt/format.h>

namespace ky {

std::vector<const char*> VulkanContext::required_instance_extensions_names() {
    unsigned int glfw_extension_count = 0;
    const char** glfw_extension_name = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    unsigned int count =
        glfw_extension_count + CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES.size();
    std::vector<const char*> extensions(count);

    size_t j = 0;
    for (size_t i = 0; i < count; i++) {
        if (i < glfw_extension_count) {
            extensions[i] = glfw_extension_name[i];
        } else {
            extensions[i] = CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES[j];
            j++;
        }
    }
    return extensions;
}

std::vector<std::string> VulkanContext::available_instance_extensions_names() {
    unsigned int count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> properties(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());

    std::vector<std::string> extensions;
    for (size_t i = 0; i < properties.size(); i++) {
        extensions.push_back(properties[i].extensionName);
    }

    return extensions;
}

VulkanContext::VulkanContext(const std::string_view& app_name, WindowManager& window_manager)
        : _window_manager(&window_manager),
          _app_info({
              .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
              .pNext = nullptr,
              .pApplicationName = app_name.data(),
              .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
              .pEngineName = "Kryos Engine",
              .engineVersion = VK_MAKE_VERSION(1, 0, 0),
              .apiVersion = VK_API_VERSION_1_0,
          }) {
    _init_instance();
}

void VulkanContext::shutdown() {
    vkDestroyInstance(_instance, nullptr);
}

bool VulkanContext::_init_instance() {
    std::vector<const char*> extensions = required_instance_extensions_names();

    VkInstanceCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = &_app_info;

    info.enabledLayerCount = extensions.size();
    info.ppEnabledLayerNames = extensions.data();

    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = nullptr;

    KY_ERROR_CONDITION_MSG_RETURN(vkCreateInstance(&info, nullptr, &_instance) == VK_SUCCESS,
                                  false, "Failed to create vulkan instance");
    return true;
}

} // namespace ky
