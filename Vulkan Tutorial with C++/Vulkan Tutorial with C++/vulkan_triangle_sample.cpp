﻿#include "vulkan_triangle_sample.h"
#include <vulkan/vulkan.hpp>
#include <type_traits>

void vulkan_triangle_sample::initialize_window() noexcept
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window_ = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

void vulkan_triangle_sample::generate_debug_messenger_create_info()
{
	debug_messenger_ = debug_messenger{
		{
			{},
			{
				DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
				DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		DebugUtilsMessageSeverityFlagBitsEXT::eError
			},
			{
				DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		DebugUtilsMessageTypeFlagBitsEXT::eValidation
			},
		debug_callback
		}
	};
}

void vulkan_triangle_sample::generate_instance_create_info()
{
	instance::info_type instance_create_info;
	instance_create_info.application_info = info_proxy<ApplicationInfo>{"Hello Triangle", "No Engine"};
	{
		uint32_t count;
		const auto extensions = glfwGetRequiredInstanceExtensions(&count);
		instance_create_info.extension_names.resize(count);
		std::generate(
			instance_create_info.extension_names.begin(),
			instance_create_info.extension_names.end(),
			[&, i = 0]()mutable {return extensions[i++]; }
		);
	}
	if constexpr(is_debug)
	{
		instance_create_info.layer_names = {"VK_LAYER_KHRONOS_validation"};
		if(!is_included(
			instance_create_info.layer_names,
			enumerateInstanceLayerProperties(),
			[](const auto& layer, const auto& p)->bool { return layer == p.layerName; }
		))
			throw std::runtime_error("validation layers requested, but not available!");

		generate_debug_messenger_create_info();
		instance_create_info.info.pNext = &debug_messenger_.info;
		instance_create_info.extension_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	if(!is_included(
		instance_create_info.extension_names,
		enumerateInstanceExtensionProperties(),
		[](const auto& layer, const auto& p)->bool { return layer == p.extensionName; }))
		throw std::runtime_error("requested instance extension is not available!");

	instance_create_info.set_proxy();
	instance_ = instance{instance_create_info};
}

bool vulkan_triangle_sample::generate_physical_device(const PhysicalDevice& d)
{
	size_t i = 0;
	for(const auto& p : d.getQueueFamilyProperties())
	{
		if(p.queueCount > 0)
		{
			bool done = true;

			if(graphics_queue_index_ == -1)
				if(p.queueFlags & QueueFlagBits::eGraphics)	graphics_queue_index_ = static_cast<uint32_t>(i);
				else done = false;

			if(present_queue_index_ == -1)
				if(d.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface_)) present_queue_index_ = static_cast<uint32_t>(i);
				else done = false;

			if(done) return true;
		}

		++i;
	}

	return false;
}

void vulkan_triangle_sample::generate_device_create_info()
{
	using value_type = decltype(std::declval<info_t<Device>>().queue_create_infos)::value_type;

	device_ = device{
		device::info_type{
			{
				{
					graphics_queue_index_,
					info_proxy<DeviceQueueCreateInfo>{ { 1 }, {{}, graphics_queue_index_}}
				},
				{
					present_queue_index_,
					info_proxy<DeviceQueueCreateInfo>{ { 1 }, {{}, graphics_queue_index_}}
				}
			},
		vector<string>{VK_KHR_SWAPCHAIN_EXTENSION_NAME}
	}
	};
	if(!is_included(
		device_.info.extension_names,
		physical_device_.enumerateDeviceExtensionProperties(),
		[](const auto& layer, const auto& p)->bool { return layer == p.extensionName; }
	))
		throw std::runtime_error("requested device extension is not available!");
}

void vulkan_triangle_sample::generate_swap_chain_create_info()
{
	const auto&& formats = physical_device_.getSurfaceFormatsKHR(*surface_);
	const SurfaceFormatKHR required_format{Format::eB8G8R8A8Unorm, ColorSpaceKHR::eSrgbNonlinear};
	const auto available_format = std::find(formats.cbegin(), formats.cend(),
		required_format) == formats.end() ? formats.front() : required_format;

	const auto&& present_modes = physical_device_.getSurfacePresentModesKHR(*surface_);
	const auto required_present_mode = PresentModeKHR::eMailbox;
	const auto present_mode = std::find(present_modes.cbegin(), present_modes.cend(),
		required_present_mode) == present_modes.cend() ? required_present_mode : PresentModeKHR::eFifo;

	const auto&& capabilities = physical_device_.getSurfaceCapabilitiesKHR(*surface_);

	const auto extent = capabilities.currentExtent.width == -1 ?
		Extent2D{width, height} :
		Extent2D{
		std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
		std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
	};
	auto info = swap_chain::info_type{
		{},
		{
			{},
			*surface_,
			std::min(capabilities.minImageCount + 1, capabilities.maxImageCount),
			available_format.format,
			available_format.colorSpace,
			extent,
			1,
			ImageUsageFlagBits::eColorAttachment,
			SharingMode::eConcurrent,
			0,
			nullptr,
			capabilities.currentTransform,
			CompositeAlphaFlagBitsKHR::eOpaque,
			present_mode,
			true
		}
	};

	if(device_.info.queue_create_infos.size() > 1)
	{
		for(const auto& pair : device_.info.queue_create_infos)
			info.queue_family_indices.insert(pair.first);
		info.set_proxy();
	}
	else info.info.imageSharingMode = SharingMode::eExclusive;

	swap_chain_ = swap_chain{std::move(info)};
}

void vulkan_triangle_sample::generate_image_view_create_infos()
{
	auto&& images = device_->getSwapchainImagesKHR(*swap_chain_);

	image_views_.resize(images.size());
	std::generate(
		image_views_.begin(),
		image_views_.end(),
		[&images, this, i = size_t{0}]()mutable
	{
		;
		return image_view{
			{
				{},
				std::move(images[i++]),
				ImageViewType::e2D,
				swap_chain_.info.info.imageFormat,
				ComponentMapping{},
				ImageSubresourceRange{ImageAspectFlagBits::eColor, 0, 1, 0, 1}
			}
		};
	}
	);
}

void vulkan_triangle_sample::generate_render_pass_create_info()
{
	//the first subpass deal with the color attachment
	AttachmentDescription color_attachment = {
		{},
		swap_chain_.info.info.imageFormat,
		SampleCountFlagBits::e1,
		AttachmentLoadOp::eClear,
		AttachmentStoreOp::eStore,
		AttachmentLoadOp::eDontCare,
		AttachmentStoreOp::eDontCare,
		ImageLayout::eUndefined,
		ImageLayout::ePresentSrcKHR
	};
	AttachmentReference color_attachment_ref = {0, ImageLayout::eColorAttachmentOptimal};
	auto color_attachment_subpass = info_proxy<SubpassDescription>{
		{},
		vector<AttachmentReference>{std::move(color_attachment_ref)}
	};
	SubpassDependency dependency = {
		VK_SUBPASS_EXTERNAL,
		0,
		PipelineStageFlagBits::eColorAttachmentOutput,
		PipelineStageFlagBits::eColorAttachmentOutput,
		AccessFlags{},
		AccessFlagBits::eColorAttachmentRead | AccessFlagBits::eColorAttachmentWrite,
	};

	render_pass_ = render_pass{
		render_pass::info_type{
			{std::move(color_attachment)},
			vector<info_proxy<SubpassDescription>>{std::move(color_attachment_subpass)},
			vector<SubpassDependency>{std::move(dependency)}
	}
	};
}

void vulkan_triangle_sample::generate_framebuffer_create_infos()
{
	frame_buffers_.resize(image_views_.size());
	std::generate(
		frame_buffers_.begin(),
		frame_buffers_.end(),
		[this, it = std::move(image_views_.cbegin())]()mutable
	{
		return frame_buffer{
			frame_buffer::info_type{
				{**it++},
				{
					{},
					*render_pass_,
					0,
					nullptr,
					swap_chain_.info.info.imageExtent.width,
					swap_chain_.info.info.imageExtent.height,
					1
				}
		}
		};
	}
	);
}

void vulkan_triangle_sample::generate_shader_module_create_infos()
{
	CompileOptions options;
	const auto vertex_shader_code_path = path("shaders") / path("shader.vert");
	const auto fragment_shader_code_path = path("shaders") / path("shader.frag");
	options.SetGenerateDebugInfo();
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	cfin.open(vertex_shader_code_path);
	if(!cfin) throw std::runtime_error("failed to load vertex code file\n");
	csout << cfin.rdbuf();
	{
		const auto&& vertex_code_compile_res = glsl_compile_to_spriv(
			csout.str(),
			shaderc_vertex_shader,
			"vertex",
			options
		);
		if(std::get<2>(vertex_code_compile_res) != shaderc_compilation_status_success)
			throw std::runtime_error("vertex code compile failure\n" + std::get<1>(vertex_code_compile_res));
		vertex_shader_module_ = shader_module{shader_module::info_type{std::move(std::get<0>(vertex_code_compile_res)), {}}};
	}

	cfin.close();
	cfin.open(fragment_shader_code_path);
	if(!cfin) throw std::runtime_error("failed to load fragment code file\n");
	csout.str("");
	csout << cfin.rdbuf();
	{
		const auto&& fragment_compile_res = glsl_compile_to_spriv(
			csout.str(),
			shaderc_fragment_shader,
			"fragment",
			options
		);
		if(std::get<2>(fragment_compile_res) != shaderc_compilation_status_success)
			throw std::runtime_error("fragment code compile failure\n" + std::get<1>(fragment_compile_res));
		fragment_shader_module_ = shader_module{shader_module::info_type{std::move(std::get<0>(fragment_compile_res)), {}}};
	}
	cfin.close();
}

//For now we don't need to do anything about the create present_info.
void vulkan_triangle_sample::generate_pipeline_layout_create_info()
{}

void vulkan_triangle_sample::generate_graphics_pipeline_create_info()
{
	auto vertex_shader_stage = info_proxy<PipelineShaderStageCreateInfo>{
		"main",
		nullopt,
		PipelineShaderStageCreateInfo{{}, ShaderStageFlagBits::eVertex, *vertex_shader_module_}
	};
	auto fragment_shader_stage = info_proxy<PipelineShaderStageCreateInfo>{
		"main",
		nullopt,
		PipelineShaderStageCreateInfo{{}, ShaderStageFlagBits::eFragment, *fragment_shader_module_}
	};

	PipelineInputAssemblyStateCreateInfo input_assembly_state = {{}, PrimitiveTopology::eTriangleList};
	auto viewport_state = info_proxy<PipelineViewportStateCreateInfo>{
		{
			{
				0,
				0,
				static_cast<float>(swap_chain_.info.info.imageExtent.width),
				static_cast<float>(swap_chain_.info.info.imageExtent.height),
				0,
				1
			}
		},
		vector<Rect2D>{Rect2D{{0, 0}, swap_chain_.info.info.imageExtent}}
	};

	PipelineRasterizationStateCreateInfo rasterization_state = {
		{},
		false,
		false,
		PolygonMode::eFill,
		CullModeFlagBits::eBack,
		FrontFace::eClockwise,
		false,
		0,
		0,
		0,
		1
	};
	auto color_blend_state = info_proxy<PipelineColorBlendStateCreateInfo>{
		{
			{
				false,
				BlendFactor::eZero,
				BlendFactor::eZero,
				BlendOp::eAdd,
				BlendFactor::eZero,
				BlendFactor::eZero,
				BlendOp::eAdd,
				ColorComponentFlagBits::eA | ColorComponentFlagBits::eR |
				ColorComponentFlagBits::eG | ColorComponentFlagBits::eB
			}
		},
		{
			{},
			false,
			LogicOp::eCopy,
			0,
			nullptr,
			std::array<float, 4>{0, 0, 0, 0}
		}
	};
	GraphicsPipelineCreateInfo info;

	info.renderPass = *render_pass_;
	info.layout = *pipeline_layout_;

	graphics_pipeline_ = graphics_pipeline{
		graphics_pipeline::info_type{
			{std::move(vertex_shader_stage), std::move(fragment_shader_stage)},
			info_proxy<PipelineVertexInputStateCreateInfo>{},
			std::move(input_assembly_state),
			nullopt,
			std::move(viewport_state),
			std::move(rasterization_state),
			PipelineMultisampleStateCreateInfo{},
			nullopt,
			std::move(color_blend_state),
			nullopt,
			std::move(info)
	}
	};
}

void vulkan_triangle_sample::generate_graphics_command_pool_create_info()
{
	graphics_command_pool_ = command_pool{
		{
			{},
			graphics_queue_index_
		}
	};
}

void vulkan_triangle_sample::generate_graphics_command_buffer_allocate_info()
{
	graphics_command_buffers_.resize(frame_buffers_.size());
	for(auto& buffer : graphics_command_buffers_)
		buffer = command_buffer{
			{
				*graphics_command_pool_,
				CommandBufferLevel::ePrimary,
				static_cast<decltype(buffer.info.commandBufferCount)>(frame_buffers_.size())
			}
	};
}

void vulkan_triangle_sample::generate_render_info()
{
	command_buffer_begin_info_ = info_proxy<CommandBufferBeginInfo>{
		nullopt,
		CommandBufferBeginInfo{CommandBufferUsageFlagBits::eSimultaneousUse}
	};

	render_pass_begin_infos_.resize(frame_buffers_.size());
	submit_infos_proxy_.resize(graphics_command_buffers_.size());
	submit_infos_.resize(submit_infos_proxy_.size());
	present_infos_.resize(submit_infos_.size());

	std::for_each(
		graphics_command_buffers_.begin(),
		graphics_command_buffers_.end(),
		[this, i = size_t{0}](auto& buffer)mutable
	{
		render_pass_begin_infos_[i] = info_proxy<RenderPassBeginInfo>{
			{{std::array<float, 4>{0, 0, 0, 0}}},
			{
				RenderPass{*render_pass_},
				Framebuffer{*frame_buffers_[i]},
				Rect2D{{0, 0}, {swap_chain_.info.info.imageExtent}}
			}
		};
		try
		{
			buffer->begin(command_buffer_begin_info_.info);
			buffer->beginRenderPass(render_pass_begin_infos_[i].info, SubpassContents::eInline);
			buffer->bindPipeline(PipelineBindPoint::eGraphics, *graphics_pipeline_);
			buffer->draw(3, 1, 0, 0);
			buffer->endRenderPass();
			buffer->end();

			submit_infos_proxy_[i] = info_proxy<SubmitInfo>{
				vector<Semaphore>{*swapchain_image_syn_},
				PipelineStageFlagBits::eColorAttachmentOutput,
				vector<CommandBuffer>{*buffer},
				vector<Semaphore>{*render_syn_}
			};

			submit_infos_[i] = submit_infos_proxy_[i].info;

			present_infos_[i] = info_proxy<PresentInfoKHR>{
				vector<Semaphore>{*render_syn_},
				vector<SwapchainKHR>{*swap_chain_},
				vector<uint32_t>{static_cast<uint32_t>(i)}
			};

			i++;
		}
		catch(const std::exception& e) { std::cerr << e.what() << "\nfailed to record command buffer!"; return; }
	}
	);
}

template<typename T>
PhysicalDevice vulkan_triangle_sample::ini_physical_device(const Instance& instance, const T& t)
{
	for(const auto& device : instance.enumeratePhysicalDevices())
		if(t(device)) return device;
	throw std::runtime_error("failed to find a suitable physical device!");
}

VkBool32 vulkan_triangle_sample::debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT flag_bits,
	VkDebugUtilsMessageTypeFlagsEXT  type_flags,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void*
)
{
	std::cerr << to_string(DebugUtilsMessageSeverityFlagBitsEXT{flag_bits}) << '\n' <<
		to_string(DebugUtilsMessageTypeFlagsEXT{type_flags}) << '\n' <<
		"validation layer: " << p_callback_data->pMessage << '\n';
	return VK_FALSE;
}

void vulkan_triangle_sample::initialize_vulkan()
{
	generate_instance_create_info();
	initialize_instance(instance_);
	if constexpr(is_debug)
		initialize_debug_messenger(instance_, debug_messenger_);

	initialize_surface(instance_, surface_, window_);

	physical_device_ = initialize_physical_device(
		*instance_,
		[this](const auto& d) { return generate_physical_device(d); }
	);

	generate_device_create_info();
	initialize_device(physical_device_, device_, *instance_);

	graphics_queue_ = device_->getQueue(graphics_queue_index_, 0);
	present_queue_ = device_->getQueue(present_queue_index_, 0);

	generate_swap_chain_create_info();
	initialize_swap_chain(device_, swap_chain_);

	generate_image_view_create_infos();
	for(auto& view : image_views_)
		initialize_image_view(device_, view);

	generate_render_pass_create_info();
	initialize_render_pass(device_, render_pass_);

	generate_framebuffer_create_infos();
	for(auto& fb : frame_buffers_)
		initialize_framebuffer(device_, fb);

	generate_shader_module_create_infos();
	initialize_shader_module(device_, vertex_shader_module_);
	initialize_shader_module(device_, fragment_shader_module_);

	generate_pipeline_layout_create_info();
	initialize_pipeline_layout(device_, pipeline_layout_);

	generate_graphics_pipeline_create_info();
	{
		vector<decltype(graphics_pipeline_)> graphics_pipelines_(1);
		graphics_pipelines_.front() = std::move(graphics_pipeline_);
		initialize_graphics_pipelines(device_, graphics_pipelines_);
		graphics_pipeline_ = std::move(graphics_pipelines_.front());
	}

	generate_graphics_command_pool_create_info();
	initialize_command_pool(device_, graphics_command_pool_);

	generate_graphics_command_buffer_allocate_info();
	initialize_command_buffers(device_, graphics_command_buffers_);

	initialize_semaphore(device_, swapchain_image_syn_);

	initialize_semaphore(device_, render_syn_);

	initialize_fence(device_, gpu_syn_);
	generate_render_info();
}

void vulkan_triangle_sample::re_initialize_vulkan()
{
	{
		int width = 0, height = 0;
		while((width | height) == 0)
		{
			glfwGetFramebufferSize(window_, &width, &height);
			glfwWaitEvents();
		}
	}
	device_->waitIdle();
	device_->resetFences({*gpu_syn_});

	objects_reset(graphics_command_buffers_, device_);

	graphics_pipeline_ = nullptr;

	pipeline_layout_ = nullptr;

	for(auto& buffer : frame_buffers_)
		buffer = nullptr;

	render_pass_ = nullptr;

	for(auto& view : image_views_)
		view = nullptr;

	generate_swap_chain_create_info();
	initialize_swap_chain(device_, swap_chain_);

	generate_image_view_create_infos();
	for(auto& view : image_views_)
		initialize_image_view(device_, view);

	generate_render_pass_create_info();
	initialize_render_pass(device_, render_pass_);

	generate_framebuffer_create_infos();
	for(auto& fb : frame_buffers_)
		initialize_framebuffer(device_, fb);

	generate_pipeline_layout_create_info();
	initialize_pipeline_layout(device_, pipeline_layout_);

	generate_graphics_pipeline_create_info();
	{
		vector<decltype(graphics_pipeline_)> graphics_pipelines_(1);
		graphics_pipelines_.front() = std::move(graphics_pipeline_);
		initialize_graphics_pipelines(device_, graphics_pipelines_);
		graphics_pipeline_ = std::move(graphics_pipelines_.front());
	}

	generate_graphics_command_buffer_allocate_info();
	initialize_command_buffers(device_, graphics_command_buffers_);

	generate_render_info();
}

bool vulkan_triangle_sample::render()
{
	device_->resetFences({*gpu_syn_});
	try
	{
		const auto index = device_->acquireNextImageKHR(
			*swap_chain_,
			-1,
			*swapchain_image_syn_,
			*gpu_syn_
		).value;
		device_->waitForFences({*gpu_syn_}, true, -1);
		graphics_queue_.submit(submit_infos_[index], nullptr);
		present_queue_.presentKHR(present_infos_[index]);
	}
	catch(const SystemError& error)
	{
		if(error.code() == Result::eErrorOutOfDateKHR || error.code() == Result::eSuboptimalKHR)
		{
			re_initialize_vulkan();
			return true;
		}
		std::cerr << error.what();
		return false;
	}
	catch(const std::exception& e) { std::cerr << e.what(); return false; }
	return true;
}

void vulkan_triangle_sample::main_loop()  noexcept
{
	while(!glfwWindowShouldClose(window_) && render())
		glfwPollEvents();
}

void vulkan_triangle_sample::glfw_cleanup() noexcept
{
	glfwDestroyWindow(window_);
	glfwTerminate();
	window_ = nullptr;
}

vulkan_triangle_sample::~vulkan_triangle_sample()
{
	device_->waitIdle();
	glfw_cleanup();
}

void vulkan_triangle_sample::run()
{
	initialize_window();
	initialize_vulkan();
	main_loop();
}