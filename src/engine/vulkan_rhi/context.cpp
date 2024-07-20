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

#include "vulkan_rhi/context.h"
#include "core/console.h"
#include "vulkan_rhi/window_handle.h"
#include <GLFW/glfw3.h>
#include <fmt/format.h>

namespace ky {

VulkanContext::VulkanContext(WindowHandle* window, const std::string_view& title, int window_width,
                             int window_height, int window_opts)
{
    _instance.init(title);
    window->init(this, title, window_width, window_height, window_opts);

    // Initialize device
    {
        std::vector<VkPhysicalDevice> devices(VulkanDevice::available_physical_devices(_instance));
        VkPhysicalDevice physical_device;

        VulkanDevice::pick_physical_device(&physical_device, window->surface, &_queue_families,
                                           devices);
        KY_VULKAN_CONDITION_FATAL(physical_device != nullptr,
                                  "Failed to find suitable physical device");
        _device.init(_instance, physical_device, _queue_families);
    }

    _queue_families.init_queues(_device);
}

void VulkanContext::shutdown()
{
    _device.shutdown();
    _instance.shutdown();
}

} // namespace ky
