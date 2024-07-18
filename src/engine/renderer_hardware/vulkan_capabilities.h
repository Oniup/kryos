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

#ifndef KRYOS_RENDERER_HARDWARE__VULKAN_CAPABILITIES_H
#define KRYOS_RENDERER_HARDWARE__VULKAN_CAPABILITIES_H

#include <vector>
#include <vulkan/vulkan.h>

namespace ky {

struct VulkanAttributes {
#ifndef NDEBUG
    static bool check_required_validation_layers(const std::vector<const char*>& required);
#endif
    static bool check_required_instance_extensions(const std::vector<const char*>& required);

#ifndef NDEBUG
    static std::vector<const char*> required_validation_layers();
#endif
    static std::vector<const char*> required_instance_extensions(bool validation_layers);

#ifndef NDEBUG
    static std::vector<VkLayerProperties> available_validation_layers();
#endif
    static std::vector<VkExtensionProperties> available_instance_extensions();
};

} // namespace ky

#endif
