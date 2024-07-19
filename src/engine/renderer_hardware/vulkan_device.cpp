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

std::vector<VkQueueFamilyProperties>
    VulkanQueueFamilies::available_physical_device_queue_families(VkPhysicalDevice physical_device)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_famlies(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, queue_famlies.data());
    return queue_famlies;
}

void VulkanQueueFamilies::init_required_indices(VkPhysicalDevice physical_device)
{
    std::vector<VkQueueFamilyProperties> families =
        available_physical_device_queue_families(physical_device);

    uint32_t index = 0;
    for (const VkQueueFamilyProperties& family : families) {
        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = index;
        }
        index++;
    }
}

void VulkanQueueFamilies::init_queues(VkDevice device)
{
    vkGetDeviceQueue(device, indices.graphics, 0, &graphics);
}

bool VulkanQueueFamilies::is_valid() const
{
    return indices.graphics != Indices::INVALID;
}

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

void VulkanDevice::pick_physical_device(VkPhysicalDevice* physical_device,
                                        VulkanQueueFamilies* queue_families,
                                        VkPhysicalDeviceFeatures* features,
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

        VulkanQueueFamilies families;
        families.init_required_indices(device);
        if (!families.is_valid()) {
            continue;
        }
        *queue_families = std::move(families);

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
                *features = device_features;
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

bool VulkanDevice::init(VulkanInstance& instance)
{
    KY_VULKAN_CONDITION_ERROR_RETURN(instance.instance != nullptr, false,
                                     "Initialize instance before initalizing vulkan");
    std::vector<VkPhysicalDevice> devices(available_physical_devices(instance));

    VkPhysicalDeviceFeatures device_features;
    pick_physical_device(&physical_device, &queue_families, &device_features, devices);
    KY_VULKAN_CONDITION_FATAL(physical_device != nullptr,
                              "Failed to find suitable physical device");

    float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_families.indices.graphics;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.pEnabledFeatures = &device_features;

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

    VkResult result = vkCreateDevice(physical_device, &create_info, nullptr, &device);
    KY_VULKAN_CONDITION_ERROR_RETURN(result == VK_SUCCESS, false,
                                     "Failed to create physical device");

    queue_families.init_queues(device);
    return true;
}

void VulkanDevice::shutdown()
{
    vkDestroyDevice(device, nullptr);
}

} // namespace ky
