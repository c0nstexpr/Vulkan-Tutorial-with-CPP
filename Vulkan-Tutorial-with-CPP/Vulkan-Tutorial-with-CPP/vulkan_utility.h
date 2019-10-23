#pragma once

#include "vulkan_object.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS

namespace vulkan
{
	using namespace shaderc;
	using namespace glm;

	template<typename T>
	inline constexpr auto whole_size = static_cast<T>(VK_WHOLE_SIZE);

	template<typename InfoType>
	void initialize_instance(
		object<Instance, InfoType>& instance,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		instance = object<Instance, InfoType>{
			instance.info(),
			createInstance(
				instance.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>
			)
		};
	}

	template<typename T>
	[[nodiscard]] PhysicalDevice initialize_physical_device(const Instance& instance, const T& t)
	{
		for(const auto& device : instance.enumeratePhysicalDevices())
			if(t(device))return device;
		return PhysicalDevice{};
	}

	template<typename InfoType>
	void initialize_device(
		const PhysicalDevice& physical_device,
		object<Device, InfoType>& device,
		const Instance& instance,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		device = object<Device, InfoType>{
			device.info(),
			physical_device.createDevice(
				device.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>
			),
			instance
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_debug_messenger(
		const object<Instance, OwnerInfoType>& instance,
		object<DebugUtilsMessengerEXT, InfoType>& debug_messenger_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		debug_messenger_object = object<DebugUtilsMessengerEXT, InfoType>{
			debug_messenger_object.info(),
			instance->createDebugUtilsMessengerEXT(
				debug_messenger_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				instance.dispatch()
			),
			collector_type<debug_messenger>{instance, allocator}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_surface(
		const object<Instance, OwnerInfoType>& instance,
		object<SurfaceKHR, InfoType>& surface_object,
		GLFWwindow* window,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		VkSurfaceKHR surface_khr = nullptr;
		if(allocator)
		{
			VkAllocationCallbacks vk_allocator = *allocator;
			glfwCreateWindowSurface(*instance, window, &vk_allocator, &surface_khr);
		}
		else glfwCreateWindowSurface(*instance, window, nullptr, &surface_khr);
		surface_object = object<SurfaceKHR, InfoType>{surface_object.info(), surface_khr, collector_type<surface>{instance}};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_swap_chain(
		const object<Device, OwnerInfoType>& device,
		object<SwapchainKHR, InfoType>& swap_chain_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		swap_chain_object = object<SwapchainKHR, InfoType>{
			swap_chain_object.info(),
			device->createSwapchainKHR(
				swap_chain_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<swap_chain>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_image_view(
		const object<Device, OwnerInfoType>& device,
		object<ImageView, InfoType>& image_view_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		image_view_object = object<ImageView, InfoType>{
			image_view_object.info(),
			device->createImageView(
				image_view_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<image_view>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_render_pass(
		const object<Device, OwnerInfoType>& device,
		object<RenderPass, InfoType>& render_pass_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		render_pass_object = object<RenderPass, InfoType>{
			render_pass_object.info(),
			device->createRenderPass(
				render_pass_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<render_pass>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_shader_module(
		const object<Device, OwnerInfoType>& device,
		object<ShaderModule, InfoType>& shader_module_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		shader_module_object = object<ShaderModule, InfoType>{
			shader_module_object.info(),
			device->createShaderModule(
				shader_module_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<shader_module>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_descriptor_set_layout(
		const object<Device, OwnerInfoType>& device,
		object<DescriptorSetLayout, InfoType>& descriptor_set_layout_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		descriptor_set_layout_object = object<DescriptorSetLayout, InfoType>{
			descriptor_set_layout_object.info(),
			device->createDescriptorSetLayout(
				descriptor_set_layout_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<descriptor_set_layout>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_descriptor_pool(
		const object<Device, OwnerInfoType>& device,
		object<DescriptorPool, InfoType>& descriptor_pool_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		descriptor_pool_object = object<DescriptorPool, InfoType>{
			descriptor_pool_object.info(),
			device->createDescriptorPool(
				descriptor_pool_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<descriptor_set_layout>{device}
		};
	}

	template<
		typename InfoType,
		typename OwnerInfoType,
		typename Allocator = decltype(Device().allocateDescriptorSets(DescriptorSetAllocateInfo()))::allocator_type
	>
		void initialize_descriptor_sets(
			const object<Device, OwnerInfoType>& device,
			vector<object<DescriptorSet, InfoType>>& descriptor_set_objects,
			optional<Allocator> allocator = nullopt
		)
	{
		if(!descriptor_set_objects.size()) return;

		const DescriptorSetAllocateInfo& allocate_info = descriptor_set_objects.front().info();
		auto&& descriptor_sets = allocator ?
			device->allocateDescriptorSets(allocate_info, *allocator, device.dispatch()) :
			device->allocateDescriptorSets(allocate_info, device.dispatch());
		std::transform(
			descriptor_sets.begin(),
			descriptor_sets.end(),
			descriptor_set_objects.begin(),
			[&](decltype(*descriptor_sets.begin()) set)
			{
				return object<DescriptorSet, InfoType>{
					allocate_info,
						std::move(set),
						collector_type<descriptor_set>{device, allocate_info.descriptorPool}
				};
			}
		);
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_pipeline_layout(
		const object<Device, OwnerInfoType>& device,
		object<PipelineLayout, InfoType>& pipeline_layout_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		pipeline_layout_object = object<PipelineLayout, InfoType>{
			pipeline_layout_object.info(),
			device->createPipelineLayout(
				pipeline_layout_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<pipeline_layout>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_graphics_pipelines(
		const object<Device, OwnerInfoType>& device,
		vector<object<Pipeline, InfoType>>& graphics_pipeline_objects,
		const PipelineCache& cache = {},
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		vector<GraphicsPipelineCreateInfo> graphics_pipeline_create_infos;
		graphics_pipeline_create_infos.resize(graphics_pipeline_objects.size());
		std::transform(
			graphics_pipeline_objects.cbegin(),
			graphics_pipeline_objects.cend(),
			graphics_pipeline_create_infos.begin(),
			[](decltype(*graphics_pipeline_objects.cbegin()) object) ->
			decltype(graphics_pipeline_create_infos)::value_type{return object.info();}
		);
		{
			auto&& pipelines = device->createGraphicsPipelines(
				cache,
				graphics_pipeline_create_infos,
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			);
			auto&& it = pipelines.begin();

			for(auto& pipeline_object : graphics_pipeline_objects)
				pipeline_object = object<Pipeline, InfoType>{
				pipeline_object.info(),
				std::move(*it++),
				collector_type<graphics_pipeline>{device}
			};
		}
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_framebuffer(
		const object<Device, OwnerInfoType>& device,
		object<Framebuffer, InfoType>& frame_buffer_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		frame_buffer_object = object<Framebuffer, InfoType>{
			frame_buffer_object.info(),
			device->createFramebuffer(
				frame_buffer_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<frame_buffer>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_command_pool(
		const object<Device, OwnerInfoType>& device,
		object<CommandPool, InfoType>& command_pool_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		command_pool_object = object<CommandPool, InfoType>{
			command_pool_object.info(),
			device->createCommandPool(
				command_pool_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<command_pool>{device}
		};
	}

	template<
		typename InfoType,
		typename OwnerInfoType,
		typename Allocator = decltype(Device().allocateCommandBuffers(CommandBufferAllocateInfo()))::allocator_type
	>
		void initialize_command_buffers(
			const object<Device, OwnerInfoType>& device,
			vector<object<CommandBuffer, InfoType>>& command_buffer_objects,
			optional<Allocator> allocator = nullopt
		)
	{
		if(!command_buffer_objects.size()) return;

		const CommandBufferAllocateInfo& allocate_info = command_buffer_objects.front().info();
		auto&& buffers = allocator ?
			device->allocateCommandBuffers(allocate_info, *allocator, device.dispatch()) :
			device->allocateCommandBuffers(allocate_info, device.dispatch());
		std::transform(
			buffers.begin(),
			buffers.end(),
			command_buffer_objects.begin(),
			[&](decltype(*buffers.begin()) buffer)
			{
				return object<CommandBuffer, InfoType>{
					allocate_info,
						std::move(buffer),
						collector_type<command_buffer>{device, allocate_info.commandPool }
				};
			}
		);
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_semaphore(
		const object<Device, OwnerInfoType>& device,
		object<Semaphore, InfoType>& semaphore_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		semaphore_object = object<Semaphore, InfoType>{
			semaphore_object.info(),
			device->createSemaphore(
				semaphore_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<semaphore>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_fence(
		const object<Device, OwnerInfoType>& device,
		object<Fence, InfoType>& fence_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		fence_object = object<Fence, InfoType>{
			fence_object.info(),
			device->createFence(
				fence_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<semaphore>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_buffer(
		const object<Device, OwnerInfoType>& device,
		object<Buffer, InfoType>& buffer_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		buffer_object = object<Buffer, InfoType>{
			buffer_object.info(),
			device->createBuffer(
				buffer_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			object_collector<Buffer>{device}
		};
	}

	template<typename InfoType, typename OwnerInfoType>
	void allocate_memory(
		const object<Device, OwnerInfoType>& device,
		object<DeviceMemory, InfoType>& memory_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		memory_object = object<DeviceMemory, InfoType>{
			memory_object.info(),
			device->allocateMemory(
				memory_object.info(),
				allocator ? *allocator : null_opt<const AllocationCallbacks>,
				device.dispatch()
			),
			collector_type<device_memory>{device}
		};
	}

	template<typename MemoryInfoType, typename BufferInfoType, typename OwnerInfoType>
	object<DeviceMemory, MemoryInfoType> allocate_buffer_memory(
		const object<Device, OwnerInfoType>& device,
		const object<Buffer, BufferInfoType>& buffer_object,
		const PhysicalDevice& physical_device,
		const MemoryPropertyFlags property_flags
	)
	{
		object<DeviceMemory, MemoryInfoType> memory_object;
		const auto& requirements = device->getBufferMemoryRequirements(*buffer_object);
		decltype(memory_object.info().memoryTypeIndex) index = 0;
		decltype(requirements.memoryTypeBits) index_bit = 1;
		const auto& properties = physical_device.getMemoryProperties(device.dispatch());

		for(index = 0; index < properties.memoryTypeCount; ++index)
		{
			if(
				(requirements.memoryTypeBits & index_bit) &&
				((properties.memoryTypes[index].propertyFlags & property_flags) == property_flags)
				)
				break;
			index_bit <<= 1;
		}

		memory_object = object<DeviceMemory, MemoryInfoType>{MemoryInfoType{requirements.size, index}};
		allocate_memory(device, memory_object);
		device->bindBufferMemory(*buffer_object, *memory_object, 0, device.dispatch());
		return memory_object;
	}

	template<typename PoolObjectRangeType, typename InfoType, typename OwnerHandleType>
	void pool_objects_reset(
		PoolObjectRangeType& objects,
		const object<OwnerHandleType, InfoType>& owner
	)
	{
		using handle_type = typename PoolObjectRangeType::value_type::handle_type;

		vector<handle_type> handles{objects.size()};
		std::transform(
			objects.cbegin(),
			objects.cend(),
			handles.begin(),
			[](decltype(*objects.cbegin()) object) { return *object; }
		);

		owner->free(objects.front().getPool(), handles, owner.dispatch());
		for(auto& object : objects) *object = nullptr;
	}

	template<typename MemoryInfoType, typename OwnerInfoType, typename Input>
	void write(
		object<DeviceMemory, MemoryInfoType>& memory_object,
		const object<Device, OwnerInfoType>& device,
		const Input& data_begin,
		const Input& data_end,
		const DeviceSize offset = 0,
		const MemoryMapFlags flag = {}
	)
	{
		using data_element_type = std::decay_t<decltype(*data_begin)>;

		size_t data_size = sizeof(data_element_type) * std::distance(data_begin, data_end);

		if(offset + data_size > static_cast<MemoryAllocateInfo>(memory_object.info()).allocationSize)
			throw std::runtime_error("data size is out of destination memory range");

		std::copy(data_begin, data_end, static_cast<data_element_type*>(device->mapMemory(
			*memory_object,
			offset,
			data_size,
			flag,
			device.dispatch()
		)));
		device->unmapMemory(*memory_object);
	}

	template<typename MemoryInfoType, typename OwnerInfoType, typename T>
	void write(
		object<DeviceMemory, MemoryInfoType>& memory_object,
		const object<Device, OwnerInfoType>& device,
		const T& data,
		const DeviceSize offset = 0,
		const MemoryMapFlags flag = {}
	)
	{
		decltype(auto) data_ptr = std::addressof(data);
		write(memory_object, device, data_ptr, data_ptr + 1, offset, flag);
	}

	[[nodiscard]] inline tuple<vector<AssemblyCompilationResult::element_type>, string,
		shaderc_compilation_status> glsl_compile_to_spriv_asm(
			const string&,
			const shaderc_shader_kind,
			const char[],
			const CompileOptions & = {});

	[[nodiscard]] tuple<vector<SpvCompilationResult::element_type>, string, shaderc_compilation_status>
		glsl_compile_to_spriv(
			const string&,
			const shaderc_shader_kind,
			const char[],
			const CompileOptions & = {}
	);

	template<typename T>
	struct format;

	template<>
	struct format<float32> { static constexpr auto value = Format::eR32Sfloat; };

	template<>
	struct format<float64> { static constexpr auto value = Format::eR64Sfloat; };

	template<>
	struct format<vec2> { static constexpr auto value = Format::eR32G32Sfloat; };

	template<>
	struct format<ivec2> { static constexpr auto value = Format::eR32G32Sint; };

	template<>
	struct format<vec3> { static constexpr auto value = Format::eR32G32B32Sfloat; };

	template<>
	struct format<vec4> { static constexpr auto value = Format::eR32G32B32A32Sfloat; };

	template<>
	struct format<uvec4> { static constexpr auto value = Format::eR64Sfloat; };

	template<typename T>
	constexpr auto format_v = format<T>::value;

	template<typename T = void>
	struct index_type;

	template<>
	struct index_type<uint32_t> { static constexpr auto value = IndexType::eUint32; };

	template<>
	struct index_type<uint16_t> { static constexpr auto value = IndexType::eUint16; };

	template<>
	struct index_type<uint8_t> { static constexpr auto value = IndexType::eUint8EXT; };

	template<>
	struct index_type<void> { static constexpr auto value = IndexType::eNoneNV; };

	template<typename T = void>
	constexpr auto index_type_v = index_type<T>::value;

	struct vertex
	{
		vec2 pos;
		vec3 color;

		static const VertexInputBindingDescription description;
		static const array<VertexInputAttributeDescription, 2> attribute_descriptions;
	};
}