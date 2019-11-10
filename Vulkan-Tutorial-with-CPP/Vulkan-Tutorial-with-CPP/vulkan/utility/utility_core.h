#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC true
#define VULKAN_HPP_DEFAULT_DISPATCHER {}
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL false
#include <vulkan/vulkan.hpp>
#include "utility/utility.h"

namespace vulkan
{
    PFN_vkVoidFunction get_instance_proc_addr(const VkInstance, const char*) noexcept;
}
