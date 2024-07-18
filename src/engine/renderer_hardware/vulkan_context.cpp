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

#include "renderer_hardware/vulkan_context.h"
#include "core/console.h"
#include "renderer_hardware/vulkan_capabilities.h"
#include <GLFW/glfw3.h>
#include <fmt/format.h>

namespace ky {

#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void*)
{
    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        KY_VULKAN_ERROR(callback_data->pMessage);
    } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        KY_VULKAN_ERROR(callback_data->pMessage);
    } else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        KY_VULKAN_ERROR(callback_data->pMessage);
    } else {
        KY_VULKAN_VERBOSE(callback_data->pMessage);
    }
    return VK_FALSE;
}
#endif

VulkanContext::VulkanContext(const std::string_view& app_name, WindowManager& window_manager)
      : _window_manager(&window_manager)
{
#ifndef NDEBUG
    bool validation_layers_enabled;
    KY_VULKAN_CONDITION_FATAL(_init_instance(app_name, validation_layers_enabled),
                              "Failed to create instance");
    if (validation_layers_enabled) {
        KY_VULKAN_CONDITION_FATAL(_init_validation_layers(), "Failed to create debug messenger");
    }
#else
    KY_VULKAN_CONDITION_FATAL(_init_instance(app_name), "Failed to create instance");
#endif
}

void VulkanContext::shutdown()
{
#ifndef NDEBUG
    auto destroy_debug_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        _instance, "vkDestroyDebugUtilsMessengerEXT");
    if (destroy_debug_messenger != nullptr) {
        destroy_debug_messenger(_instance, _debug_messenger, nullptr);
    } else {
        KY_VULKAN_ERROR("Failed to destroy vulkan debug messenger: Could not find "
                        "vkDebugDestroyUtilsMessengerEXT");
    }
#endif

    vkDestroyInstance(_instance, nullptr);
}

#ifndef NDEBUG
bool VulkanContext::_init_instance(const std::string_view& app_name,
                                   bool& validation_layers_enabled)
#else
bool VulkanContext::_init_instance(const std::string_view& app_name)
#endif
{
#ifndef NDEBUG
    std::vector<const char*> validation_layers = VulkanCapabilities::required_validation_layers();
    if (VulkanCapabilities::check_required_validation_layers(validation_layers)) {
        validation_layers_enabled = true;
    } else {
        KY_VULKAN_ERROR("Validation layers are disabled: Not all required layers "
                        "are available");
        validation_layers_enabled = false;
    }

    std::vector<const char*> extensions =
        VulkanCapabilities::required_instance_extensions(validation_layers_enabled);
    KY_VULKAN_CONDITION_ERROR_RETURN(
        VulkanCapabilities::check_required_instance_extensions(extensions), false,
        "Failed to create instance. Not all required extensions are available");

    std::printf("\nLoading instance extensions:\n");
    for (const char* name : extensions) {
        std::printf(" - %s\n", name);
    }
#else
    std::vector<const char*> extensions = VulkanAttributes::required_instance_extensions(false);
    KY_VULKAN_CONDITION_ERROR_RETURN(
        VulkanAttributes::check_required_instance_extensions(extensions), false,
        "Failed to create instance: Not all required extensions are available");
#endif

    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = app_name.data();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Kryos Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo info {};
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = &app_info;

#ifdef __APPLE__
    info.flags = |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    info.enabledExtensionCount = (uint32_t)extensions.size();
    info.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
    VkDebugUtilsMessengerCreateInfoEXT debug_info = _debug_messenger_create_info();
    if (validation_layers_enabled) {
        info.enabledLayerCount = (uint32_t)validation_layers.size();
        info.ppEnabledLayerNames = validation_layers.data();
        info.pNext = &debug_info;
    } else {
        info.enabledLayerCount = 0;
        info.ppEnabledLayerNames = nullptr;
    }
#else
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = nullptr;
#endif

    KY_VULKAN_CONDITION_ERROR_RETURN(vkCreateInstance(&info, nullptr, &_instance) == VK_SUCCESS,
                                     false, "Failed to create instance");
    return true;
}

#ifndef NDEBUG
bool VulkanContext::_init_validation_layers()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info = _debug_messenger_create_info();

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        _instance, "vkCreateDebugUtilsMessengerEXT");
    KY_VULKAN_CONDITION_ERROR_RETURN(func != nullptr, false,
                                     "Failed to load debug utils messenger extenion. Could "
                                     "not find PFN_vkCreateDebugUtilsMessengerEXT");

    VkResult result = func(_instance, &create_info, nullptr, &_debug_messenger);
    KY_VULKAN_CONDITION_ERROR_RETURN(result == VK_SUCCESS, false,
                                     "Failed to initialize debug utils messenger");
    return true;
}

VkDebugUtilsMessengerCreateInfoEXT VulkanContext::_debug_messenger_create_info()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.pNext = nullptr;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_callback;
    create_info.pUserData = nullptr;
    return create_info;
}
#endif

} // namespace ky
