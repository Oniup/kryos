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

#include "renderer_hardware/vulkan_device.h"
#include "core/console.h"
#include <fmt/ranges.h>

namespace ky {

std::vector<VkPhysicalDevice> VulkanDevice::available_physical_devices(VulkanInstance& instance)
{
    uint32_t count;
    vkEnumeratePhysicalDevices(instance.instance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(instance.instance, &count, devices.data());
    return devices;
}

VkPhysicalDevice VulkanDevice::pick_physical_device(const std::vector<VkPhysicalDevice>& devices)
{
    KY_VULKAN_CONDITION_ERROR_RETURN(devices.size() > 0, nullptr,
                                     "No physical devices available, cannot initialize vulkan");

    int physical_device_score = 0;
    VkPhysicalDevice physical_device = nullptr;
    for (VkPhysicalDevice device : devices) {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);

        // NOTE: This list of requirements will grow overtime
        if (!features.geometryShader && !features.sparseBinding) {
            continue;
        }

        int score = 0;
        constexpr int DISCRETE_GPU_SCORE = 1000;
        constexpr int INTEGRATED_GPU_SCORE = 10;
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += DISCRETE_GPU_SCORE;
        } else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            score += INTEGRATED_GPU_SCORE;
        }

        if (score > INTEGRATED_GPU_SCORE) {
            // NOTE: This list of requirements will grow overtime
            score += properties.limits.maxImageDimension2D;
            score += properties.limits.maxUniformBufferRange;
            score += properties.limits.maxMemoryAllocationCount;

            if (score > physical_device_score) {
                physical_device = device;
                physical_device_score = score;
            }
        }
    }

    return physical_device;
}

void VulkanDevice::init(VulkanInstance& instance)
{
    KY_VULKAN_CONDITION_FATAL(instance.instance != nullptr,
                              "Initialize instance before initalizing vulkan");

    std::vector<VkPhysicalDevice> devices(available_physical_devices(instance));
    VkPhysicalDevice physical_device = pick_physical_device(devices);
    KY_VULKAN_CONDITION_FATAL(physical_device != nullptr,
                              "Failed to find suitable physical device");
}

void VulkanDevice::shutdown(VulkanInstance& instance)
{
}

} // namespace ky
