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

#ifndef KRYOS_RENDERER_HARDWARE__VULKAN_DEVICE_H
#define KRYOS_RENDERER_HARDWARE__VULKAN_DEVICE_H

#include "renderer_hardware/vulkan_instance.h"
#include <limits>
#include <vector>
#include <vulkan/vulkan.h>

namespace ky {

struct VulkanQueueFamilies {
    struct Indices {
        static constexpr uint32_t INVALID = std::numeric_limits<uint32_t>::max();
        uint32_t graphics = Indices::INVALID;
    };

public:
    Indices indices;
    VkQueue graphics;

    static std::vector<VkQueueFamilyProperties>
        available_physical_device_queue_families(VkPhysicalDevice physical_device);

    void init_required_indices(VkPhysicalDevice physical_device);
    void init_queues(VkDevice device);

    bool is_valid() const;
};

struct VulkanDevice {
    VkPhysicalDevice physical_device = nullptr;
    VkDevice device = nullptr;
    VulkanQueueFamilies queue_families;

    static bool check_required_extensions(const std::vector<const char*> required);
    static std::vector<const char*> required_extensions();

    static std::vector<VkExtensionProperties>
        available_extensions(VkPhysicalDevice physical_device);
    static std::vector<VkPhysicalDevice>
        available_physical_devices(const VulkanInstance& instance);

    static void pick_physical_device(VkPhysicalDevice* physical_device,
                                     VulkanQueueFamilies* queue_families,
                                     VkPhysicalDeviceFeatures* features,
                                     const std::vector<VkPhysicalDevice>& devices);

    static void print_capabilities();
    static void print_available_physical_devices();

    bool init(VulkanInstance& instance);
    void shutdown();
};

} // namespace ky

#endif
