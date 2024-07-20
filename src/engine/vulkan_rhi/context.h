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

#ifndef KRYOS_VULKAN_RHI__CONTEXT_H
#define KRYOS_VULKAN_RHI__CONTEXT_H

#include "vulkan_rhi/device.h"
#include "vulkan_rhi/instance.h"
#include "vulkan_rhi/queue.h"
#include <string_view>
#include <vulkan/vulkan.h>

namespace ky {

class VulkanContext {
    friend struct WindowHandle;

public:
    VulkanContext(struct WindowHandle* window, const std::string_view& title, int window_width,
                  int window_height, int window_opts = 0);
    void shutdown();

private:
    VulkanInstance _instance;
    VulkanDevice _device;
    VulkanQueueFamilies _queue_families;
};

} // namespace ky

#endif
