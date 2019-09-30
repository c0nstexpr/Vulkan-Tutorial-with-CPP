#pragma once

#include"vulkan_info.h"

namespace vulkan
{
	template<typename T>
	inline auto null_opt = Optional<T>(nullptr);

	template<typename HandleType>
	using default_deleter_type = typename UniqueHandleTraits<HandleType, DispatchLoaderDefault>::deleter;

	template<typename HandleType>
	struct owner
	{
		using handle_type = HandleType;
		using type = decltype(std::declval<default_deleter_type<HandleType>>().getOwner());
	};

	template<>
	struct owner<Instance>
	{
		using handle_type = Instance;
		using type = empty_type;
	};

	template<>
	struct owner<Device>
	{
		using handle_type = Device;
		using type = empty_type;
	};

	template<typename HandleType>
	using owner_type = typename owner<HandleType>::type;

	template<typename HandleType>
	using dispatch_type = std::conditional_t<
		std::is_same_v<owner_type<HandleType>, empty_type>,
		DispatchLoaderStatic,
		DispatchLoaderDynamic
	>;

	template<typename HandleType>
	using deleter_type = typename UniqueHandleTraits<HandleType, dispatch_type<HandleType>>::deleter;

	template<typename HandleType, typename InfoType = info_t<HandleType>>
	class object_traits :public UniqueHandle<HandleType, dispatch_type<HandleType>>
	{
	public:
		using base = UniqueHandle<HandleType, dispatch_type<HandleType>>;
		using handle_type = HandleType;
		using info_type = InfoType;
		using owner_type = owner_type<handle_type>;
		using dispatch_type = dispatch_type<handle_type>;
		using deleter_type = deleter_type<handle_type>;

		using base::base;

		object_traits() = default;
		object_traits(std::nullptr_t) :object_traits() {}

		template<typename T = info_type, typename = enable_if_convertible<T, info_type>>
		explicit object_traits(
			T&& info,
			const handle_type& handle = handle_type{},
			const deleter_type& deleter = deleter_type{}
		) : base(handle, deleter), info(std::forward<T>(info))
		{}
		info_type info;
	};

	template<typename HandleType, typename InfoType = info_t<HandleType>>
	class object : public object_traits<HandleType, InfoType>
	{
	public:
		using base = object_traits<HandleType, InfoType>;
		using handle_type = typename base::handle_type;
		using info_type = typename base::info_type;
		using owner_type = typename base::owner_type;
		using dispatch_type = typename base::dispatch_type;
		using deleter_type = typename base::deleter_type;
		using base::base;

		object() = default;

		template<
			typename T = info_type,
			typename = enable_if_convertible_t<T, info_type>
		>
			explicit object(T&& info) : base(std::forward<T>(info)) {}

		template<typename T = info_type, typename = enable_if_convertible_t<T, info_type>>
		object(T&& info, const handle_type& handle, const deleter_type& deleter) :
			base(std::forward<T>(info), handle, deleter)
		{}
	};

	template<typename InfoType>
	class object<Instance, InfoType> : public object_traits<Instance, InfoType>
	{
	public:
		using base = object_traits<Instance, InfoType>;
		using handle_type = typename base::handle_type;
		using info_type = typename base::info_type;
		using owner_type = typename base::owner_type;
		using dispatch_type = typename base::dispatch_type;
		using deleter_type = typename base::deleter_type;

	private:
		DispatchLoaderDynamic dispatch_;

	public:
		object() = default;

		template<typename T = info_type, typename = enable_if_convertible_t<T, info_type>>
		explicit object(T&& info) : base(std::forward<T>(info)) {}

		template<typename T = info_type, typename = enable_if_convertible_t<T, info_type>>
		object(T&& info, const handle_type& handle) : base(std::forward<T>(info), handle), dispatch_(handle) {}

		const auto& dispatch()const { return dispatch_; }
	};

	template<typename InfoType>
	class object<Device, InfoType> : public object_traits<Device, InfoType>
	{
	public:
		using base = object_traits<Device, InfoType>;
		using handle_type = typename base::handle_type;
		using info_type = typename base::info_type;
		using owner_type = typename base::owner_type;
		using dispatch_type = typename base::dispatch_type;
		using deleter_type = typename base::deleter_type;

	private:
		DispatchLoaderDynamic dispatch_;

	public:
		object() = default;

		template<typename T = info_type, typename = enable_if_convertible_t<T, info_type>>
		object(T&& info, const handle_type& handle, const Instance& instance) :
			base(std::forward<T>(info), handle),
			dispatch_{instance, handle}
		{}

		template<typename T = info_type, typename = enable_if_convertible_t<T, info_type>>
		explicit object(T&& info) : base(std::forward<T>(info)) {}

		const auto& dispatch()const { return dispatch_; }
	};

	using instance = object<Instance>;
	using device = object<Device>;
	using debug_messenger = object<DebugUtilsMessengerEXT>;
	using surface = object<SurfaceKHR>;
	using swap_chain = object<SwapchainKHR>;
	using image_view = object<ImageView>;
	using render_pass = object<RenderPass>;
	using frame_buffer = object<Framebuffer>;
	using shader_module = object<ShaderModule>;
	using pipeline_layout = object<PipelineLayout>;
	using vertex_buffer = object<VertexBuffer::handle_type, info_t<VertexBuffer>>;
	using staging_buffer = object<StagingBuffer::handle_type, info_t<StagingBuffer>>;
	using device_memory = object<DeviceMemory>;
	using graphics_pipeline = object<GraphicsPipeline::handle_type, info_t<GraphicsPipeline>>;
	using command_pool = object<CommandPool>;
	using command_buffer = object<CommandBuffer>;
	using semaphore = object<Semaphore>;
	using fence = object<Fence>;
	using descriptor_set = object<DescriptorSet>;

	template<typename HandleType>
	class collector_traits
	{
	public:
		using deleter_type = deleter_type<HandleType>;
		using handle_type = HandleType;
		using owner_type = owner_type<handle_type>;

	protected:
		deleter_type deleter_;

	public:
		template<typename T, typename = enable_if_convertible<T, deleter_type>>
		collector_traits(T&& deleter = T{}) :deleter_(deleter) {}

		operator deleter_type& () { return deleter_; }
		operator const deleter_type& ()const { return deleter_; }
	};

	template<
		typename HandleType,
		typename = std::enable_if_t<
		std::is_same_v<
		deleter_type<HandleType>,
		ObjectDestroy<owner_type<HandleType>, dispatch_type<HandleType>>
		> ||
		std::is_same_v<
		deleter_type<HandleType>,
		ObjectFree<owner_type<HandleType>, dispatch_type<HandleType>>
		>,
		deleter_type<HandleType>
		>
	>
		class object_collector : public collector_traits<HandleType>
	{
	public:
		using handle_type = HandleType;
		using base = collector_traits<handle_type>;
		using deleter_type = typename base::deleter_type;
		using owner_type = typename base::owner_type;

		template<typename InfoType>
		explicit object_collector(
			const object<owner_type, InfoType>& object,
			const optional<const AllocationCallbacks>& allocator = nullopt
		) : base(deleter_type{*object, allocator ? *allocator : null_opt<const AllocationCallbacks>, object.dispatch()})
		{}
	};

	template<
		typename HandleType,
		typename PoolType = decltype(std::declval<deleter_type<HandleType>>().getPool()),
		typename = enable_if_same<
		deleter_type<HandleType>,
		PoolFree<owner_type<HandleType>, PoolType, dispatch_type<HandleType>>
		>
	>
		class pool_object_collector : public collector_traits<HandleType>
	{
	public:
		using handle_type = HandleType;
		using base = collector_traits<handle_type>;
		using deleter_type = typename base::deleter_type;
		using owner_type = typename base::owner_type;
		using pool_type = PoolType;

		pool_object_collector() = default;

		template<typename InfoType>
		pool_object_collector(
			const object<owner_type, InfoType>& object,
			const pool_type& pool
		) : base(deleter_type{*object, pool, object.dispatch()})
		{}
	};

	template<typename ObjectType>
	struct collector
	{
		using object_type = ObjectType;
		using type = object_collector<typename object_type::handle_type>;
	};

	template<typename InfoType>
	struct collector<object<CommandBuffer, InfoType>>
	{
		using object_type = object<CommandBuffer, InfoType>;
		using type = pool_object_collector<typename object_type::handle_type>;
	};

	template<typename InfoType>
	struct collector<object<DescriptorSet, InfoType>>
	{
		using object_type = object<DescriptorSet, InfoType>;
		using type = pool_object_collector<typename object_type::handle_type>;
	};

	template<typename ObjectType>
	using collector_type = typename collector<ObjectType>::type;
}