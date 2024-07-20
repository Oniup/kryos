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

#include "vulkan_rhi/device.h"
#include "core/console.h"
#include <fmt/ranges.h>
#include <set>

namespace ky {

bool VulkanDevice::check_required_extensions(const std::vector<const char*> required)
{
    // TODO: ....
    return false;
}

std::vector<const char*> VulkanDevice::required_extensions()
{
    // TODO: ....
    return {};
}

std::vector<VkExtensionProperties>
    VulkanDevice::available_extensions(VkPhysicalDevice physical_device)
{
    // TODO: ....
    return {};
}

std::vector<VkPhysicalDevice>
    VulkanDevice::available_physical_devices(const VulkanInstance& instance)
{
    uint32_t count;
    vkEnumeratePhysicalDevices(instance.instance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(instance.instance, &count, devices.data());
    return devices;
}

void VulkanDevice::pick_physical_device(VkPhysicalDevice* physical_device, VkSurfaceKHR surface,
                                        VulkanQueueFamilies* queue_families,
                                        const std::vector<VkPhysicalDevice>& devices)
{
    KY_VULKAN_CONDITION_ERROR(devices.size() > 0,
                              "No physical devices available, cannot initialize vulkan");

    int physical_device_score = 0;
    for (VkPhysicalDevice device : devices) {
        VkPhysicalDeviceProperties device_properties;
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        vkGetPhysicalDeviceFeatures(device, &device_features);

        // NOTE: This list of requirements will grow overtime
        if (!device_features.geometryShader && !device_features.sparseBinding) {
            continue;
        }

        queue_families->init_required_indices(device, surface);
        if (!queue_families->validate_indices()) {
            continue;
        }

        int score = 0;
        constexpr int DISCRETE_GPU_SCORE = 1000;
        constexpr int INTEGRATED_GPU_SCORE = 10;
        if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += DISCRETE_GPU_SCORE;
        }
        else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            score += INTEGRATED_GPU_SCORE;
        }

        if (score > INTEGRATED_GPU_SCORE) {
            // NOTE: This list of requirements will grow overtime
            score += device_properties.limits.maxImageDimension2D;
            score += device_properties.limits.maxUniformBufferRange;
            score += device_properties.limits.maxMemoryAllocationCount;

            if (score > physical_device_score) {
                *physical_device = device;
                physical_device_score = score;
            }
        }
    }
}

void VulkanDevice::print_capabilities()
{
}

void VulkanDevice::print_available_physical_devices()
{
}

bool VulkanDevice::init(VulkanInstance& instance, VkPhysicalDevice physical_device,
                        const VulkanQueueFamilies& queue_families)
{
    KY_VULKAN_CONDITION_ERROR_RETURN(instance.instance != nullptr, false,
                                     "Initialize instance before initalizing vulkan");

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physical_device, &features);

    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pEnabledFeatures = &features;

    create_info.enabledExtensionCount = 0;

#ifndef NDEBUG
    std::vector<const char*> validation_layers = VulkanInstance::required_validation_layers();
    if (instance.debug_messenger != nullptr) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    }
    else {
        create_info.enabledLayerCount = 0;
    }
#else
    create_info.enabledLayerCount = 0;
#endif

    float queue_priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    for (const auto& [type, queue] : queue_families.queues) {
        VkDeviceQueueCreateInfo info {};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = queue.index_family;
        info.queueCount = 1;
        info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(info);
    }
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();

    VkResult result = vkCreateDevice(physical_device, &create_info, nullptr, &device);
    KY_VULKAN_CONDITION_ERROR_RETURN(result == VK_SUCCESS, false,
                                     "Failed to create physical device");

    this->physical_device = physical_device;
    return true;
}

void VulkanDevice::shutdown()
{
    vkDestroyDevice(device, nullptr);
}

} // namespace ky
