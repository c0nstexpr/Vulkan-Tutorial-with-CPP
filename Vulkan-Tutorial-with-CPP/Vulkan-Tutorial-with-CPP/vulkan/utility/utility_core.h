#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC true
#define VULKAN_HPP_DEFAULT_DISPATCHER {}
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL false
#include <vulkan/vulkan.hpp>
#include "utility/utility.h"

namespace vulkan
{
	using namespace vk;
	using namespace utility;

	// ReSharper disable CppInconsistentNaming
	struct GraphicsPipeline : Pipeline { using base = Pipeline; using base::base; };
	struct ComputePipeline : Pipeline { using base = Pipeline; using base::base; };

	// ReSharper restore CppInconsistentNaming

	PFN_vkVoidFunction get_instance_proc_addr(const VkInstance, const char*) noexcept;
}
