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
#include <vector>
#include <vulkan/vulkan.h>

namespace ky {

struct VulkanDevice
{
    VkPhysicalDevice physical_device = nullptr;
    VkDevice device = nullptr;

    static std::vector<VkPhysicalDevice> available_physical_devices(VulkanInstance& instance);
    static VkPhysicalDevice pick_physical_device(const std::vector<VkPhysicalDevice>& devices);

    void init(VulkanInstance& instance);
    void shutdown(VulkanInstance& instance);
};

} // namespace ky

#endif
