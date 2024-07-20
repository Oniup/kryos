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

#include "vulkan_rhi/queue.h"
#include "core/console.h"
#include "vulkan_rhi/device.h"
#include <fmt/ranges.h>

namespace ky {

std::vector<VkQueueFamilyProperties>
    VulkanQueueFamilies::available_physical_device_queue_families(VkPhysicalDevice physical_device)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_famlies(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, queue_famlies.data());
    return queue_famlies;
}

void VulkanQueueFamilies::init_required_indices(VkPhysicalDevice physical_device,
                                                VkSurfaceKHR surface)
{
    std::vector<VkQueueFamilyProperties> families =
        available_physical_device_queue_families(physical_device);

    uint32_t index = 0;
    for (const VkQueueFamilyProperties& family : families) {
        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queues[(VulkanQueueType)VK_QUEUE_GRAPHICS_BIT] =
                VulkanQueue {.index_family = index, .instance = nullptr};
        }
        index++;
    }
}

void VulkanQueueFamilies::init_queues(VulkanDevice& device)
{
    for (auto& [type, queue] : queues) {
        vkGetDeviceQueue(device.device, queue.index_family, 0, &queue.instance);
    }
    KY_VULKAN_VERBOSE("Initialized queues:\n\t* {}", fmt::join(queues, "\n\t* "));
}

bool VulkanQueueFamilies::validate_indices() const
{
    for (const auto& [type, queue] : queues) {
        if (queue.index_family == VulkanQueue::INVALID_INDEX) {
            return false;
        }
    }
    return true;
}

bool VulkanQueueFamilies::validate() const
{
    for (const auto& [type, queue] : queues) {
        if (queue.index_family == VulkanQueue::INVALID_INDEX || queue.instance == nullptr) {
            return false;
        }
    }
    return true;
}

} // namespace ky
