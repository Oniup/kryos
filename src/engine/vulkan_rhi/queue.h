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

#ifndef KRYOS_VULKAN_RHI__QUEUE_H
#define KRYOS_VULKAN_RHI__QUEUE_H

#include <fmt/core.h>
#include <limits>
#include <map>
#include <vector>
#include <vulkan/vulkan.h>

namespace ky {

enum class VulkanQueueType {
    INVALID = -1,
    PRESET = 0,
    GRAPHICS = 0x00000001,
    COMPUTE = 0x00000002,
    TRANSFER = 0x00000004,
    SPARSE_BINDING = 0x00000008,
    PROTECTED = 0x00000010,
    VIDEO_DECODE = 0x00000020,
    VIDEO_ENCODE = 0x00000040,
    OPTICAL_FLOW = 0x00000100,
};

struct VulkanQueue {
    static constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();

    uint32_t index_family = INVALID_INDEX;
    VkQueue instance = nullptr;
};

struct VulkanQueueFamilies {
    std::map<VulkanQueueType, VulkanQueue> queues;

    static std::vector<VkQueueFamilyProperties>
        available_physical_device_queue_families(VkPhysicalDevice physical_device);

    void init_required_indices(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
    void init_queues(struct VulkanDevice& device);

    bool validate_indices() const;
    bool validate() const;
};

} // namespace ky

template <>
struct fmt::formatter<ky::VulkanQueueType> {
    template <typename _ParseContext>
    static constexpr auto parse(_ParseContext& cntx)
    {
        return cntx.begin();
    }

    template <typename _FormatContext>
    static auto format(const ky::VulkanQueueType& queue_type, _FormatContext& cntx)
    {
        const char* str = nullptr;
        switch (queue_type) {
            case ky::VulkanQueueType::INVALID:
                str = "Invalid";
                break;
            case ky::VulkanQueueType::PRESET:
                str = "Preset";
                break;
            case ky::VulkanQueueType::GRAPHICS:
                str = "Graphics";
                break;
            case ky::VulkanQueueType::COMPUTE:
                str = "Compute";
                break;
            case ky::VulkanQueueType::TRANSFER:
                str = "Transfer";
                break;
            case ky::VulkanQueueType::SPARSE_BINDING:
                str = "Sparse Bindings";
                break;
            case ky::VulkanQueueType::PROTECTED:
                str = "Protected";
                break;
            case ky::VulkanQueueType::VIDEO_DECODE:
                str = "Video Decode";
                break;
            case ky::VulkanQueueType::VIDEO_ENCODE:
                str = "Video Encode";
                break;
            case ky::VulkanQueueType::OPTICAL_FLOW:
                str = "Optical Flow";
                break;
        }
        return fmt::format_to(cntx.out(), "{}", str);
    }
};

template <>
struct fmt::formatter<ky::VulkanQueue> {
    template <typename _ParseContext>
    static constexpr auto parse(_ParseContext& cntx)
    {
        return cntx.begin();
    }

    template <typename _FormatContext>
    static auto format(const ky::VulkanQueue& queue, _FormatContext& cntx)
    {
        return fmt::format_to(cntx.out(), "Index Family: {}", queue.index_family);
    }
};

#endif
