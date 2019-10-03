#pragma once

#include "vulkan_object.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLFW_INCLUDE_VULKAN

namespace vulkan
{
	using namespace shaderc;

	template<typename InfoType>
	void initialize_instance(
		object<Instance, InfoType>& instance,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			instance = object<Instance, InfoType>{
				instance.info,
				createInstance(
					instance.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>
				)
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
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
		try
		{
			device = object<Device, InfoType>{
				device.info,
				physical_device.createDevice(
					device.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>
				),
				instance
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_debug_messenger(
		const object<Instance, OwnerInfoType>& instance,
		object<DebugUtilsMessengerEXT, InfoType>& debug_messenger_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			debug_messenger_object = object<DebugUtilsMessengerEXT, InfoType>{
				debug_messenger_object.info,
				instance->createDebugUtilsMessengerEXT(
					debug_messenger_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					instance.dispatch()
				),
				collector_type<debug_messenger>{instance, allocator}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
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
		try
		{
			if(allocator)
			{
				VkAllocationCallbacks vk_allocator = *allocator;
				glfwCreateWindowSurface(*instance, window, &vk_allocator, &surface_khr);
			}
			else glfwCreateWindowSurface(*instance, window, nullptr, &surface_khr);
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }

		surface_object = object<SurfaceKHR, InfoType>{surface_object.info, surface_khr, collector_type<surface>{instance}};
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_swap_chain(
		const object<Device, OwnerInfoType>& device,
		object<SwapchainKHR, InfoType>& swap_chain_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			swap_chain_object = object<SwapchainKHR, InfoType>{
				swap_chain_object.info,
				device->createSwapchainKHR(
					swap_chain_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<swap_chain>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_image_view(
		const object<Device, OwnerInfoType>& device,
		object<ImageView, InfoType>& image_view_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			image_view_object = object<ImageView, InfoType>{
				image_view_object.info,
				device->createImageView(
					image_view_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<image_view>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_render_pass(
		const object<Device, OwnerInfoType>& device,
		object<RenderPass, InfoType>& render_pass_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			render_pass_object = object<RenderPass, InfoType>{
				render_pass_object.info,
				device->createRenderPass(
					render_pass_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<render_pass>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_shader_module(
		const object<Device, OwnerInfoType>& device,
		object<ShaderModule, InfoType>& shader_module_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			shader_module_object = object<ShaderModule, InfoType>{
				shader_module_object.info,
				device->createShaderModule(
					shader_module_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<shader_module>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_pipeline_layout(
		const object<Device, OwnerInfoType>& device,
		object<PipelineLayout, InfoType>& pipeline_layout_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			pipeline_layout_object = object<PipelineLayout, InfoType>{
				pipeline_layout_object.info,
				device->createPipelineLayout(
					pipeline_layout_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<pipeline_layout>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
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
		std::generate(
			graphics_pipeline_create_infos.begin(),
			graphics_pipeline_create_infos.end(),
			[&, it = std::move(graphics_pipeline_objects.cbegin())]()mutable {return GraphicsPipelineCreateInfo{it++->info}; }
		);
		try
		{
			std::for_each(
				graphics_pipeline_objects.begin(),
				graphics_pipeline_objects.end(),
				[&, it = std::move(device->createGraphicsPipelines(
					cache,
					graphics_pipeline_create_infos,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				).cbegin())](auto& graphics_pipeline_object)mutable
			{
				graphics_pipeline_object = object<Pipeline, InfoType>{
					graphics_pipeline_object.info,
					std::move(*it++),
					collector_type<graphics_pipeline>{device}
				};
			}
			);
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_framebuffer(
		const object<Device, OwnerInfoType>& device,
		object<Framebuffer, InfoType>& frame_buffer_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			frame_buffer_object = object<Framebuffer, InfoType>{
				frame_buffer_object.info,
				device->createFramebuffer(
					frame_buffer_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<frame_buffer>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_command_pool(
		const object<Device, OwnerInfoType>& device,
		object<CommandPool, InfoType>& command_pool_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			command_pool_object = object<CommandPool, InfoType>{
				command_pool_object.info,
				device->createCommandPool(
					command_pool_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<command_pool>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_buffer(
		const object<Device, OwnerInfoType>& device,
		object<Buffer, InfoType>& buffer_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			buffer_object = object<Buffer, InfoType>{
				buffer_object.info,
				device->createBuffer(
					buffer_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				object_collector<Buffer>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void allocate_memory(
		const object<Device, OwnerInfoType>& device,
		object<DeviceMemory, InfoType>& memory_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			memory_object = object<DeviceMemory, InfoType>{
				memory_object.info,
				device->allocateMemory(
					memory_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<device_memory>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType, typename Allocator>
	void initialize_command_buffers(
		const object<Device, OwnerInfoType>& device,
		const vector<object<CommandBuffer, InfoType>, Allocator>& command_buffer_objects,
		const Allocator& allocator = Allocator{}
	)
	{
		try
		{
			if(command_buffer_objects.size())
			{
				std::for_each(
					command_buffer_objects.begin(),
					command_buffer_objects.end(),
					[&, it = std::move(
						device->allocateCommandBuffers(command_buffer_objects.front().info, allocator, device.dispatch())
					).cbegin()](auto& command_buffer_object)mutable
				{
					command_buffer_object = object<CommandBuffer, InfoType>{
						command_buffer_object.info,
						std::move(*it++),
						collector_type<command_buffer>{device}
					};
				}
				);
			}
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_command_buffers(
		const object<Device, OwnerInfoType>& device,
		vector<object<CommandBuffer, InfoType>>& command_buffer_objects
	)
	{
		try
		{
			if(command_buffer_objects.size())
			{
				const CommandBufferAllocateInfo& allocate_info = command_buffer_objects.front().info;
				std::for_each(
					command_buffer_objects.begin(),
					command_buffer_objects.end(),
					[&, it = std::move(
						device->allocateCommandBuffers(
							allocate_info,
							device.dispatch()).begin())](auto& command_buffer_object)mutable
				{
					command_buffer_object = object<CommandBuffer, InfoType>{
						allocate_info,
						*it++,
						collector_type<command_buffer>{device, allocate_info.commandPool }
					};
				}
				);
			}
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_semaphore(
		const object<Device, OwnerInfoType>& device,
		object<Semaphore, InfoType>& semaphore_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			semaphore_object = object<Semaphore, InfoType>{
				semaphore_object.info,
				device->createSemaphore(
					semaphore_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<semaphore>{device}
			};
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
	}

	template<typename InfoType, typename OwnerInfoType>
	void initialize_fence(
		const object<Device, OwnerInfoType>& device,
		object<Fence, InfoType>& fence_object,
		const optional<AllocationCallbacks>& allocator = nullopt
	)
	{
		try
		{
			fence_object = object<Fence, InfoType>{
				fence_object.info,
				device->createFence(
					fence_object.info,
					allocator ? *allocator : null_opt<const AllocationCallbacks>,
					device.dispatch()
				),
				collector_type<semaphore>{device}
			};
		}
		catch(const SystemError & err) { std::cout << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cout << "Exception: " << e.what() << '\n'; }
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
		try
		{
			const auto& requirements = device->getBufferMemoryRequirements(*buffer_object);
			decltype(memory_object.info.memoryTypeIndex) index = 0;
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

			memory_object.info = MemoryInfoType{requirements.size, index};
			allocate_memory(device, memory_object);
			device->bindBufferMemory(*buffer_object, *memory_object, 0, device.dispatch());
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
		return memory_object;
	}

	template<
		typename PoolObjectRangeType,
		typename InfoType,
		typename PoolType = decltype(std::declval<typename PoolObjectRangeType::value_type>().getPool()),
		typename OwnerHandleType = typename PoolObjectRangeType::value_type::owner_type
	>
		void objects_reset(
			PoolObjectRangeType& objects,
			const object<OwnerHandleType, InfoType>& owner
		)
	{
		using handle_type = typename PoolObjectRangeType::value_type::handle_type;

		if(objects.size())
		{
			vector<handle_type> object_handles{objects.size()};
			std::generate(
				object_handles.begin(),
				object_handles.end(),
				[it = std::move(objects.cbegin())]()mutable {return **it++; }
			);
			owner->free(objects.front().getPool(), object_handles, owner.dispatch());

			for(auto& object : objects)*object = nullptr;
		}
	}

	template<typename MemoryInfoType, typename OwnerInfoType, typename Input>
	object<DeviceMemory, MemoryInfoType>& write(
		object<DeviceMemory, MemoryInfoType>& memory_object,
		const object<Device, OwnerInfoType>& device,
		const Input& data_begin,
		const Input& data_end,
		const DeviceSize offset = 0,
		const MemoryMapFlags flag = {}
	)
	{
		using data_element_type = std::decay_t<decltype(*data_begin)>;

		size_t data_size = 0;
		data_size = sizeof(data_element_type) * std::distance(data_begin, data_end);

		if(offset + data_size > static_cast<MemoryAllocateInfo>(memory_object.info).allocationSize)
			throw std::runtime_error("data size is out of destination memory range");

		try
		{
			std::copy(
				data_begin,
				data_end,
				static_cast<data_element_type*>(device->mapMemory(
					*memory_object,
					offset,
					data_size,
					flag,
					device.dispatch()
				))
			);
			device->unmapMemory(*memory_object);
		}
		catch(const SystemError & err) { std::cerr << "SystemError: " << err.what() << '\n'; }
		catch(const std::exception & e) { std::cerr << "Exception: " << e.what() << '\n'; }
		return memory_object;
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
	struct format<glm::float32> { inline static constexpr auto value = Format::eR32Sfloat; };

	template<>
	struct format<glm::float64> { inline static constexpr auto value = Format::eR64Sfloat; };

	template<>
	struct format<glm::vec2> { inline static constexpr auto value = Format::eR32G32Sfloat; };

	template<>
	struct format<glm::ivec2> { inline static constexpr auto value = Format::eR32G32Sint; };

	template<>
	struct format<glm::vec3> { inline static constexpr auto value = Format::eR32G32B32Sfloat; };

	template<>
	struct format<glm::vec4> { inline static constexpr auto value = Format::eR32G32B32A32Sfloat; };

	template<>
	struct format<glm::uvec4> { inline static constexpr auto value = Format::eR64Sfloat; };

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
		glm::vec2 pos;
		glm::vec3 color;

		static const VertexInputBindingDescription description;
		static const array<VertexInputAttributeDescription, 2> attribute_descriptions;
	};
}