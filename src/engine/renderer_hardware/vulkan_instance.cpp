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

#include "renderer_hardware/vulkan_instance.h"
#include "core/console.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <fmt/ranges.h>

namespace ky {

static std::vector<const char*> _CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES {};

#ifndef NDEBUG
static std::vector<const char*> _REQUIRED_VALIDATION_LAYER_NAMES {
    "VK_LAYER_KHRONOS_validation",
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void*)
{
    switch (message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            KY_VULKAN_INFO(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            KY_VULKAN_WARN(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            KY_VULKAN_ERROR(callback_data->pMessage);
            break;
        default:
            KY_VULKAN_VERBOSE(callback_data->pMessage);
            break;
    }
    return VK_FALSE;
}
#endif

void VulkanInstance::init(const std::string_view& app_name)
{
#ifndef NDEBUG
    bool validation_layers_enabled;
    bool result = _init_instance(app_name, validation_layers_enabled);
    KY_VULKAN_CONDITION_FATAL(result, "Failed to initialize instance");
    if (validation_layers_enabled) {
        result = _init_validation_layers();
        KY_VULKAN_CONDITION_FATAL(result, "Failed to initialize debug messenger");
    }
#else
    bool result = _init_instance(app_name);
    KY_VULKAN_CONDITION_FATAL(result, "Failed to create instance");
#endif
}

void VulkanInstance::shutdown()
{
#ifndef NDEBUG
    auto destroy_debug_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if (destroy_debug_messenger != nullptr) {
        destroy_debug_messenger(instance, debug_messenger, nullptr);
    }
    else {
        KY_VULKAN_ERROR("Failed to destroy vulkan debug messenger: Could not find "
                        "vkDebugDestroyUtilsMessengerEXT");
    }
#endif

    vkDestroyInstance(instance, nullptr);
}

#ifndef NDEBUG
bool VulkanInstance::check_required_validation_layers(const std::vector<const char*>& required)
{
    std::vector<VkLayerProperties> properties = available_validation_layers();

    for (const char* layer : required) {
        bool found = false;
        for (VkLayerProperties& property : properties) {
            bool compare =
                std::strncmp(layer, property.layerName, std::strlen(property.layerName)) == 0;
            if (compare) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

std::vector<const char*> VulkanInstance::required_validation_layers()
{
    return _REQUIRED_VALIDATION_LAYER_NAMES;
}

std::vector<VkLayerProperties> VulkanInstance::available_validation_layers()
{
    uint32_t count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> properties(count);
    vkEnumerateInstanceLayerProperties(&count, properties.data());
    return properties;
}

void VulkanInstance::print_validation_capabilities()
{
    std::vector<VkLayerProperties> properties = available_validation_layers();
    std::vector<const char*> names(properties.size());
    for (size_t i = 0; i < properties.size(); i++) {
        names[i] = properties[i].layerName;
    }
    KY_CONTEXT_INFO("VULKAN Validation Layer Capabilities", "\t* {}", fmt::join(names, "\n\t* "));
    KY_CONTEXT_INFO("VULKAN Required Validation Layers", "\t* {}",
                    fmt::join(VulkanInstance::required_validation_layers(), "\n\t* "));
}
#endif

bool VulkanInstance::check_required_instance_extensions(const std::vector<const char*>& required)
{
    std::vector<VkExtensionProperties> properties = available_instance_extensions();

    for (const char* layer : required) {
        bool found = false;
        for (VkExtensionProperties& property : properties) {
            bool compare = std::strncmp(layer, property.extensionName,
                                        std::strlen(property.extensionName)) == 0;
            if (compare) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

std::vector<const char*> VulkanInstance::required_instance_extensions(bool validation_layers)
{
    uint32_t glfw_extension_count = 0;
    const char** glfw_extension_names = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions(glfw_extension_names,
                                        glfw_extension_names + glfw_extension_count);

    for (const char* cross_platform : _CROSS_PLATFORM_REQUIRED_INSTANCE_EXTENSION_NAMES) {
        extensions.push_back(cross_platform);
    }

    if (validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

std::vector<VkExtensionProperties> VulkanInstance::available_instance_extensions()
{
    unsigned int count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> properties(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());
#ifdef __APPLE__
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
    return properties;
}

void VulkanInstance::print_capabilities()
{
    std::vector<VkExtensionProperties> properties = available_instance_extensions();
    std::vector<const char*> names(properties.size());
    for (size_t i = 0; i < properties.size(); i++) {
        names[i] = properties[i].extensionName;
    }
#ifndef NDEBUG
    constexpr bool validation_layers = true;
    print_validation_capabilities();
#else
    constexpr bool validation_layers = false;
#endif
    KY_CONTEXT_INFO("VULKAN Instance Extension Capabilities", "\t* {}",
                    fmt::join(names, "\n\t* "));
    KY_CONTEXT_INFO(
        "VULKAN Required Instance Extensions", "\t* {}",
        fmt::join(VulkanInstance::required_instance_extensions(validation_layers), "\n\t* "));
}

#ifndef NDEBUG
bool VulkanInstance::_init_instance(const std::string_view& app_name,
                                    bool& validation_layers_enabled)
#else
bool VulkanInstance::_init_instance(const std::string_view& app_name)
#endif
{
#ifndef NDEBUG
    std::vector<const char*> validation_layers = required_validation_layers();
    if (check_required_validation_layers(validation_layers)) {
        validation_layers_enabled = true;
    }
    else {
        KY_VULKAN_ERROR("Validation layers are disabled: Not all required layers "
                        "are available");
        validation_layers_enabled = false;
    }

    std::vector<const char*> extensions = required_instance_extensions(validation_layers_enabled);
    KY_VULKAN_CONDITION_ERROR_RETURN(
        check_required_instance_extensions(extensions), false,
        "Failed to create instance. Not all required extensions are available");
#else
    std::vector<const char*> extensions = required_instance_extensions(false);
    KY_VULKAN_CONDITION_ERROR_RETURN(
        check_required_instance_extensions(extensions), false,
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

    info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    info.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
    VkDebugUtilsMessengerCreateInfoEXT debug_info = _debug_messenger_create_info();
    if (validation_layers_enabled) {
        info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        info.ppEnabledLayerNames = validation_layers.data();
        info.pNext = &debug_info;
    }
    else {
        info.enabledLayerCount = 0;
        info.ppEnabledLayerNames = nullptr;
    }
#else
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = nullptr;
#endif
    VkResult result = vkCreateInstance(&info, nullptr, &instance);
    KY_VULKAN_CONDITION_ERROR_RETURN(result == VK_SUCCESS, false, "Failed to create instance");
    return true;
}

#ifndef NDEBUG

bool VulkanInstance::_init_validation_layers()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info = _debug_messenger_create_info();

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");
    KY_VULKAN_CONDITION_ERROR_RETURN(func != nullptr, false,
                                     "Failed to load debug utils messenger extenion. Could "
                                     "not find PFN_vkCreateDebugUtilsMessengerEXT");

    VkResult result = func(instance, &create_info, nullptr, &debug_messenger);
    KY_VULKAN_CONDITION_ERROR_RETURN(result == VK_SUCCESS, false,
                                     "Failed to initialize debug utils messenger");
    return true;
}

VkDebugUtilsMessengerCreateInfoEXT VulkanInstance::_debug_messenger_create_info()
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
