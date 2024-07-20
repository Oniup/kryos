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

#ifndef KRYOS_VULKAN_RHI__DEVICE_H
#define KRYOS_VULKAN_RHI__DEVICE_H

#include "vulkan_rhi/instance.h"
#include "vulkan_rhi/queue.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace ky {

struct VulkanDevice {
    VkPhysicalDevice physical_device = nullptr;
    VkDevice device = nullptr;

    static bool check_required_extensions(const std::vector<const char*> required);
    static std::vector<const char*> required_extensions();

    static std::vector<VkExtensionProperties>
        available_extensions(VkPhysicalDevice physical_device);
    static std::vector<VkPhysicalDevice>
        available_physical_devices(const VulkanInstance& instance);

    static void pick_physical_device(VkPhysicalDevice* physical_device, VkSurfaceKHR surface,
                                     VulkanQueueFamilies* queue_families,
                                     const std::vector<VkPhysicalDevice>& devices);

    static void print_capabilities();
    static void print_available_physical_devices();

    bool init(VulkanInstance& instance, VkPhysicalDevice physical_device,
              const VulkanQueueFamilies& queue_families);
    void shutdown();
};

} // namespace ky

#endif
