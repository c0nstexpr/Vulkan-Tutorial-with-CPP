#pragma once

#include "vulkan/utility/info/info.h"

namespace vk
{
	// ReSharper disable once CppInconsistentNaming
	using GraphicsPipeline = vulkan::GraphicsPipeline;

	template<typename Dispatch>
	class UniqueHandleTraits<GraphicsPipeline, Dispatch> : public UniqueHandleTraits<Pipeline, Dispatch>
	{
	public:
		using base = UniqueHandleTraits<Pipeline, Dispatch>;
		using base::base;
		using deleter = typename base::deleter;
	};

	template<typename Dispatch>
	class UniqueHandle<GraphicsPipeline, Dispatch> : public UniqueHandle<Pipeline, Dispatch>
	{
	public:
		using base = UniqueHandle<Pipeline, Dispatch>;
		using base::base;
		UniqueHandle(base&&) noexcept;
	};
}

namespace vulkan::utility
{
	template<typename HandleType, typename DispatchType = DispatchLoaderDynamic>
	class object_traits : public UniqueHandle<HandleType, DispatchType>
	{
	public:
		using handle_type = HandleType;
		using dispatch_type = DispatchType;
		using base = UniqueHandle<handle_type, dispatch_type>;
		using info_type = info_t<handle_type>;
		using base_info_type = typename info<handle_type>::base_info_type;

		struct pool;

	protected:
		using default_deleter_type = typename UniqueHandleTraits<handle_type, DispatchType>::deleter;

	private:
		info_type info_;

	public:
		object_traits(std::nullptr_t = {}) noexcept;
		explicit object_traits(info_type, base = base{}) noexcept;

		constexpr const auto& info() const noexcept;
	};

	template<>
	struct object_traits<CommandPool>::pool { using element_type = CommandBuffer; };

	template<>
	struct object_traits<DescriptorPool>::pool { using element_type = DescriptorSet; };
}

#include "object_traits.tpp"
