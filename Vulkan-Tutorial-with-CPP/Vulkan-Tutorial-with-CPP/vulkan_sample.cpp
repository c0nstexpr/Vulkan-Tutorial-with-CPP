#include "vulkan_sample.h"

void vulkan_sample::initialize_window() noexcept
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window_ = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

void vulkan_sample::generate_debug_messenger_create_info()
{
	using debug_messenger_type = decltype(debug_messenger_);
	using debug_messenger_info_type = debug_messenger_type::info_type;

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

void vulkan_sample::generate_instance_create_info()
{
	using instance_type = decltype(instance_);
	using instance_info_type = instance_type::info_type;
	vector<string> ext_names;
	vector<string> layer_names;
	InstanceCreateInfo info;

	{
		uint32_t count;
		const auto extensions = glfwGetRequiredInstanceExtensions(&count);
		ext_names.resize(count);
		std::transform(extensions, extensions + count, ext_names.begin(), [&](const auto& str)mutable { return str; });
	}

	if constexpr(is_debug)
	{
		layer_names.push_back("VK_LAYER_KHRONOS_validation");
		if(!is_included(
			layer_names,
			enumerateInstanceLayerProperties(),
			[](const auto& layer, const auto& p)->bool { return layer == p.layerName; }
		))
			throw std::runtime_error("validation layers requested, but not available!");

		generate_debug_messenger_create_info();
		info.pNext = &debug_messenger_.info();
		ext_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	if(!is_included(
		ext_names,
		enumerateInstanceExtensionProperties(),
		[](const auto& layer, const auto& p)->bool { return layer == p.extensionName; }))
		throw std::runtime_error("requested instance extension is not available!");

	instance_ = instance_type{instance_info_type{
		info_proxy<ApplicationInfo>{"Hello Vulkan", "No Engine"},
		std::move(ext_names),
		std::move(layer_names),
		std::move(info)
	}};
}

bool vulkan_sample::generate_physical_device(const PhysicalDevice& d)
{
	size_t i = 0;
	if(d.getProperties().deviceType == PhysicalDeviceType::eDiscreteGpu)
		for(const auto& p : d.getQueueFamilyProperties())
		{
			if(p.queueCount == 0) continue;
			bool done = true;

			if(graphics_queue_index_ == -1)
				if(p.queueFlags & QueueFlagBits::eGraphics)	graphics_queue_index_ = static_cast<uint32_t>(i);
				else done = false;

			if(present_queue_index_ == -1)
				if(d.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface_)) present_queue_index_ = static_cast<uint32_t>(i);
				else done = false;

			if(done) return true;
			++i;
		}

	return false;
}

void vulkan_sample::generate_device_create_info()
{
	using device_type = decltype(device_);
	using device_info_type = device_type::info_type;
	using set_type = decay_to_origin_t<decltype(device_.info().queue_create_infos_set_property())>;

	device_ = device_type{device_info_type{
		{
			info_proxy<DeviceQueueCreateInfo>{ { 1 }, {{}, graphics_queue_index_}},
			info_proxy<DeviceQueueCreateInfo>{ { 1 }, {{}, graphics_queue_index_}}
		},
		vector<string>{VK_KHR_SWAPCHAIN_EXTENSION_NAME}}
	};
	if(!is_included(
		device_.info().extension_name_strs_property(),
		physical_device_.enumerateDeviceExtensionProperties(),
		[](const auto& layer, const auto& p)->bool { return layer == p.extensionName; }
	))
		throw std::runtime_error("requested device extension is not available!");
}

void vulkan_sample::generate_swap_chain_create_info()
{
	using swap_chain_type = decltype(swap_chain_);
	using swap_chain_info_type = decltype(swap_chain_)::info_type;
	using set_type = decay_to_origin_t<decltype(swap_chain_.info().queue_family_indices_set_property())>;

	const auto& formats = physical_device_.getSurfaceFormatsKHR(*surface_);
	const SurfaceFormatKHR required_format{Format::eB8G8R8A8Unorm, ColorSpaceKHR::eSrgbNonlinear};
	const auto available_format = std::find(formats.cbegin(), formats.cend(),
		required_format) == formats.end() ? formats.front() : required_format;

	const auto& present_modes = physical_device_.getSurfacePresentModesKHR(*surface_);
	const auto required_present_mode = PresentModeKHR::eFifo;
	const auto present_mode =
		std::find(present_modes.cbegin(), present_modes.cend(), required_present_mode) !=
		present_modes.cend() ? required_present_mode : PresentModeKHR::eFifo;

	const auto& capabilities = physical_device_.getSurfaceCapabilitiesKHR(*surface_);

	const auto extent = capabilities.currentExtent.width == -1 ?
		Extent2D{width, height} :
		Extent2D{
		std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
		std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
	};
	auto sharing_mode = SharingMode::eConcurrent;
	set_type queue_indices;

	if(device_.info().queue_create_infos_set_property().size() > 1)
		for(const auto& info : device_.info().queue_create_infos_set_property())
			queue_indices.insert(info.info.queueFamilyIndex);
	else sharing_mode = SharingMode::eExclusive;

	swap_chain_ = swap_chain_type{swap_chain_info_type{
		std::move(queue_indices),
		SwapchainCreateInfoKHR{
			{},
			*surface_,
			std::min(capabilities.minImageCount + 1, capabilities.maxImageCount),
			available_format.format,
			available_format.colorSpace,
			extent,
			1,
			ImageUsageFlagBits::eColorAttachment,
			sharing_mode,
			0,
			nullptr,
			capabilities.currentTransform,
			CompositeAlphaFlagBitsKHR::eOpaque,
			present_mode,
			true,
	}
	}};
}

void vulkan_sample::generate_image_view_create_infos()
{
	using image_view_type = decltype(image_views_)::value_type;

	auto&& images = device_->getSwapchainImagesKHR(*swap_chain_);

	image_views_.resize(images.size());
	std::transform(
		images.begin(),
		images.end(),
		image_views_.begin(),
		[this](decltype(*images.begin()) image)mutable
		{
			return image_view_type{{
				{},
				std::move(image),
				ImageViewType::e2D,
				swap_chain_.info().info.imageFormat,
				ComponentMapping{},
				ImageSubresourceRange{ImageAspectFlagBits::eColor, 0, 1, 0, 1}
				}};
		}
	);
}

void vulkan_sample::generate_render_pass_create_info()
{
	using render_pass_type = decltype(render_pass_);
	using render_pass_info_type = decltype(render_pass_)::info_type;

	//the first subpass deal with the color attachment
	AttachmentDescription color_attachment = {
		{},
		swap_chain_.info().info.imageFormat,
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

	render_pass_ = render_pass_type{render_pass_info_type{
		{std::move(color_attachment)}, {std::move(color_attachment_subpass)}, {std::move(dependency)}}
	};
}

void vulkan_sample::generate_framebuffer_create_infos()
{
	using frame_buffer_type = decltype(frame_buffers_)::value_type;
	using frame_buffer_info_type = frame_buffer_type::info_type;

	frame_buffers_.resize(image_views_.size());
	std::transform(
		image_views_.cbegin(),
		image_views_.cend(),
		frame_buffers_.begin(),
		[this](const image_view& image_view)
		{
			return frame_buffer_type{frame_buffer_info_type{
				{*image_view},
				{
					{},
					*render_pass_,
					0,
					nullptr,
					swap_chain_.info().info.imageExtent.width,
					swap_chain_.info().info.imageExtent.height,
					1
				}
			}
			};
		}
	);
}

void vulkan_sample::generate_shader_module_create_infos()
{
	using shader_module_type = decltype(vertex_shader_module_);
	using shader_module_info_type = shader_module_type::info_type;

	CompileOptions options;
	const auto vertex_shader_code_path = path("shaders") / path("shader.vert");
	const auto fragment_shader_code_path = path("shaders") / path("shader.frag");

	options.SetGenerateDebugInfo();
	options.SetOptimizationLevel(shaderc_optimization_level_performance);

	cfin.open(vertex_shader_code_path);
	if(!cfin) throw std::runtime_error("failed to load vertex code file\n");
	csout << cfin.rdbuf();
	{
		auto&& [spriv_code, error_str, status] = glsl_compile_to_spriv(
			csout.str(),
			shaderc_vertex_shader,
			"vertex",
			options
		);
		if(status != shaderc_compilation_status_success)
			throw std::runtime_error("vertex code compile failure\n" + error_str);
		vertex_shader_module_ = shader_module_type{
			shader_module_info_type{std::move(spriv_code), {}}
		};
	}

	cfin.close();
	cfin.open(fragment_shader_code_path);
	if(!cfin) throw std::runtime_error("failed to load fragment code file\n");
	csout.str("");
	csout << cfin.rdbuf();
	{
		auto&& [spriv_code, error_str, status] = glsl_compile_to_spriv(
			csout.str(),
			shaderc_fragment_shader,
			"fragment",
			options
		);
		if(status != shaderc_compilation_status_success)
			throw std::runtime_error("fragment code compile failure\n" + error_str);
		fragment_shader_module_ = shader_module_type{
			shader_module_info_type{std::move(spriv_code)}
		};
	}
	cfin.close();
}

void vulkan_sample::generate_descriptor_set_layout_create_info()
{
	using descriptor_set_layout_type = decltype(descriptor_set_layout_);
	using descriptor_set_layout_info_type = descriptor_set_layout_type::info_type;

	descriptor_set_layout_ = descriptor_set_layout_type{descriptor_set_layout_info_type{
		{DescriptorSetLayoutBinding{0, DescriptorType::eUniformBuffer, 1, ShaderStageFlagBits::eVertex}}
	}};
}

void vulkan_sample::generate_descriptor_pool_create_info()
{
	using descriptor_pool_type = decltype(descriptor_pool_);
	using descriptor_pool_info_type = descriptor_pool_type::info_type;

	descriptor_pool_ = descriptor_pool_type{descriptor_pool_info_type{{{
			DescriptorType::eUniformBuffer,
			static_cast<uint32_t>(image_views_.size())
		}}, {DescriptorPoolCreateFlagBits::eFreeDescriptorSet}}};
}

void vulkan_sample::generate_descriptor_set_allocate_info()
{
	using descriptor_set_type = decltype(descriptor_sets_)::value_type;
	using descriptor_set_info_type = descriptor_set_type::info_type;

	descriptor_sets_.resize(image_views_.size());
	for(auto& descriptor_set : descriptor_sets_)
		descriptor_set = descriptor_set_type{descriptor_set_info_type{
			{image_views_.size(), *descriptor_set_layout_},
			descriptor_set_info_type::info_type{*descriptor_pool_}
	}};
}

void vulkan_sample::generate_pipeline_layout_create_info()
{
	using pipeline_layout_type = decltype(pipeline_layout_);
	using pipeline_layout_info_type = pipeline_layout_type::info_type;

	pipeline_layout_ = pipeline_layout_type{pipeline_layout_info_type{{*descriptor_set_layout_}}};
}

void vulkan_sample::generate_graphics_pipeline_create_info()
{
	using graphics_pipeline_type = decltype(graphics_pipeline_);
	using graphics_pipeline_info_type = graphics_pipeline_type::info_type;
	GraphicsPipelineCreateInfo info;
	info.renderPass = *render_pass_;
	info.layout = *pipeline_layout_;

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
	auto input_state = info_proxy<PipelineVertexInputStateCreateInfo>{
		{vertex::description},
		{
			vertex::attribute_descriptions.cbegin(),
			vertex::attribute_descriptions.cend()
		}
	};
	PipelineInputAssemblyStateCreateInfo input_assembly_state = {{}, PrimitiveTopology::eTriangleList};
	auto viewport_state = info_proxy<PipelineViewportStateCreateInfo>{
		{
			{
				0,
				0,
				static_cast<float>(swap_chain_.info().info.imageExtent.width),
				static_cast<float>(swap_chain_.info().info.imageExtent.height),
				0,
				1
			}
		},
		vector<Rect2D>{Rect2D{{0, 0}, swap_chain_.info().info.imageExtent}}
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
	PipelineMultisampleStateCreateInfo multi_sample_state;

	graphics_pipeline_ = graphics_pipeline_type{graphics_pipeline_info_type{
		{std::move(vertex_shader_stage), std::move(fragment_shader_stage)},
		std::move(input_state),
		std::move(input_assembly_state),
		nullopt,
		std::move(viewport_state),
		std::move(rasterization_state),
		std::move(multi_sample_state),
		nullopt,
		std::move(color_blend_state),
		nullopt,
		std::move(info)
	}};
}

void vulkan_sample::generate_transform_buffer_create_info()
{
	using uniform_buffer_type = decltype(transform_buffer_);
	using uniform_buffer_info_type = uniform_buffer_type::info_type;

	transform_buffer_ = uniform_buffer_type{
		uniform_buffer_info_type{{}, {{}, sizeof(transform), BufferUsageFlagBits::eUniformBuffer}}
	};
}

void vulkan_sample::generate_vertices_buffer_allocate_info()
{
	using vertices_buffer_type = decltype(vertices_buffer_);
	using vertices_buffer_info_type = vertices_buffer_type::info_type;

	vertices_buffer_ = vertices_buffer_type{vertices_buffer_info_type{{}, {
		{},
		vertex::description.stride * vertices.size(),
		BufferUsageFlagBits::eVertexBuffer | BufferUsageFlagBits::eTransferDst
	}}};
}

void vulkan_sample::generate_indices_buffer_allocate_info()
{
	using indices_buffer_type = decltype(indices_buffer_);
	using indices_buffer_info_type = indices_buffer_type::info_type;

	indices_buffer_ = indices_buffer_type{indices_buffer_info_type{{}, {
		{},
		sizeof indices.front() * indices.size(),
		BufferUsageFlagBits::eIndexBuffer | BufferUsageFlagBits::eTransferDst
	}}};
}

void vulkan_sample::generate_vertices_staging_buffer_allocate_info()
{
	using vertices_staging_buffer_type = decltype(vertices_staging_buffer_);
	using vertices_staging_buffer_info_type = vertices_staging_buffer_type::info_type;

	vertices_staging_buffer_ = vertices_staging_buffer_type{vertices_staging_buffer_info_type{{}, {
		{},
		vertices_buffer_.info().info.size,
		BufferUsageFlagBits::eTransferSrc | BufferUsageFlagBits::eTransferDst
	}}};
}

void vulkan_sample::generate_indices_staging_buffer_allocate_info()
{
	using indices_staging_buffer_type = decltype(indices_staging_buffer_);
	using indices_staging_buffer_info_type = indices_staging_buffer_type::info_type;

	indices_staging_buffer_ = indices_staging_buffer_type{indices_staging_buffer_info_type{{}, {
		{},
		indices_buffer_.info().info.size,
		BufferUsageFlagBits::eTransferSrc | BufferUsageFlagBits::eTransferDst
	}}};
}

void vulkan_sample::generate_graphics_command_pool_create_info()
{
	using command_pool_type = decltype(graphics_command_pool_);
	using command_pool_info_type = command_pool_type::info_type;

	graphics_command_pool_ = command_pool_type{
		command_pool_info_type{CommandPoolCreateFlagBits::eResetCommandBuffer, graphics_queue_index_}
	};
}

void vulkan_sample::generate_graphics_command_buffer_allocate_info()
{
	using command_buffer_type = decltype(graphics_command_buffers_)::value_type;
	using command_buffer_info_type = command_buffer_type::info_type;

	graphics_command_buffers_.resize(frame_buffers_.size());
	for(auto& buffer : graphics_command_buffers_)
		buffer = command_buffer_type{{
				*graphics_command_pool_,
				CommandBufferLevel::ePrimary,
				static_cast<decltype(buffer.info().commandBufferCount)>(frame_buffers_.size())
			}};
}

void vulkan_sample::generate_sync_objects_create_info()
{
	using semaphore_type = decltype(swap_chain_image_syn_)::value_type;
	using semaphore_info_type = semaphore_type::info_type;

	using fence_type = decltype(gpu_syn_)::value_type;
	using fence_info_type = fence_type::info_type;

	swap_chain_image_syn_.resize(image_views_.size());
	render_syn_.resize(image_views_.size());
	gpu_syn_.resize(image_views_.size());

	for(auto& syn : gpu_syn_)
		syn = fence_type{fence_info_type{FenceCreateFlagBits::eSignaled}};
}

void vulkan_sample::generate_render_info()
{
	for(const auto& descriptor_set : descriptor_sets_)
		device_->updateDescriptorSets({info_proxy<WriteDescriptorSet>{
			{},
			{{*transform_buffer_, 0, whole_size<decltype(DescriptorBufferInfo::range)>}},
			{},
			{*descriptor_set, 0, 0, 1, DescriptorType::eUniformBuffer}
	}}, {}, device_.dispatch());

	command_buffer_begin_info_.info = CommandBufferBeginInfo{CommandBufferUsageFlagBits::eSimultaneousUse};

	render_pass_begin_infos_.resize(frame_buffers_.size());
	submit_infos_.resize(graphics_command_buffers_.size());
	present_infos_.resize(submit_infos_.size());
	std::for_each(
		graphics_command_buffers_.begin(),
		graphics_command_buffers_.end(),
		[this, index = size_t{0}](command_buffer& buffer)mutable
	{
		render_pass_begin_infos_[index] = info_proxy<RenderPassBeginInfo>{
			{{std::array<float, 4>{0, 0, 0, 0}}},
			{
				RenderPass{*render_pass_},
				Framebuffer{*frame_buffers_[index]},
				Rect2D{{0, 0}, {swap_chain_.info().info.imageExtent}}
			}
		};

		buffer->begin(command_buffer_begin_info_.info, device_.dispatch());
		buffer->copyBuffer(
			*vertices_staging_buffer_,
			*vertices_buffer_,
			{BufferCopy{0, 0, vertices_staging_buffer_.info().info.size}},
			device_.dispatch()
		);
		buffer->copyBuffer(
			*indices_staging_buffer_,
			*indices_buffer_,
			{BufferCopy{0, 0, indices_buffer_.info().info.size}},
			device_.dispatch()
		);
		buffer->beginRenderPass(render_pass_begin_infos_[index].info, SubpassContents::eInline, device_.dispatch());
		buffer->bindPipeline(PipelineBindPoint::eGraphics, *graphics_pipeline_, device_.dispatch());
		buffer->bindVertexBuffers(0, {*vertices_buffer_}, {0}, device_.dispatch());
		buffer->bindIndexBuffer(
			{*indices_buffer_},
			{0},
			index_type_v<decltype(indices)::value_type>,
			device_.dispatch()
		);
		buffer->bindDescriptorSets(
			PipelineBindPoint::eGraphics,
			*pipeline_layout_,
			0,
			*descriptor_sets_[index],
			{},
			device_.dispatch()
		);
		buffer->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0, device_.dispatch());
		buffer->endRenderPass(device_.dispatch());
		buffer->end(device_.dispatch());

		submit_infos_[index] = info_proxy<SubmitInfo>{
			vector<Semaphore>{},
			PipelineStageFlagBits::eColorAttachmentOutput,
			vector<CommandBuffer>{*buffer},
			vector<Semaphore>{*render_syn_[index]}
		};

		present_infos_[index] = info_proxy<PresentInfoKHR>{
			submit_infos_[index].signal_semaphores_property,
			vector<SwapchainKHR>{*swap_chain_},
			vector<uint32_t>{static_cast<uint32_t>(index)}
		};

		index++;
	}
	);
}

VkBool32 vulkan_sample::debug_callback(
	const VkDebugUtilsMessageSeverityFlagBitsEXT flag_bits,
	const VkDebugUtilsMessageTypeFlagsEXT type_flags,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void*)
{
	const auto flag = DebugUtilsMessageSeverityFlagBitsEXT{flag_bits};
	std::cerr << to_string(flag) << '\n' <<
		to_string(DebugUtilsMessageTypeFlagsEXT{type_flags}) << '\n' <<
		"validation layer: " << p_callback_data->pMessage << '\n';

	return flag == DebugUtilsMessageSeverityFlagBitsEXT::eError ? true : false;
}

void vulkan_sample::initialize_vulkan()
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

	generate_descriptor_set_layout_create_info();
	initialize_descriptor_set_layout(device_, descriptor_set_layout_);

	generate_descriptor_pool_create_info();
	initialize_descriptor_pool(device_, descriptor_pool_);

	generate_descriptor_set_allocate_info();
	initialize_descriptor_sets(device_, descriptor_sets_);

	generate_pipeline_layout_create_info();
	initialize_pipeline_layout(device_, pipeline_layout_);

	generate_graphics_pipeline_create_info();
	{
		vector<decltype(graphics_pipeline_)> graphics_pipelines_(1);
		graphics_pipelines_.front() = std::move(graphics_pipeline_);
		initialize_graphics_pipelines(device_, graphics_pipelines_);
		graphics_pipeline_ = std::move(graphics_pipelines_.front());
	}

	generate_transform_buffer_create_info();
	initialize_buffer(device_, transform_buffer_);
	transform_buffer_memory_ = allocate_buffer_memory<decltype(transform_buffer_memory_)::info_type>(
		device_,
		transform_buffer_,
		physical_device_,
		MemoryPropertyFlagBits::eHostVisible
		);

	generate_vertices_buffer_allocate_info();
	initialize_buffer(device_, vertices_buffer_);
	vertices_buffer_memory_ = allocate_buffer_memory<decltype(vertices_buffer_memory_)::info_type>(
		device_,
		vertices_buffer_,
		physical_device_,
		MemoryPropertyFlagBits::eDeviceLocal
		);

	generate_indices_buffer_allocate_info();
	initialize_buffer(device_, indices_buffer_);
	indices_buffer_memory_ = allocate_buffer_memory<decltype(indices_buffer_memory_)::info_type>(
		device_,
		indices_buffer_,
		physical_device_,
		MemoryPropertyFlagBits::eDeviceLocal
		);

	generate_vertices_staging_buffer_allocate_info();
	initialize_buffer(device_, vertices_staging_buffer_);
	vertices_staging_buffer_memory_ = allocate_buffer_memory<decltype(vertices_staging_buffer_memory_)::info_type>(
		device_,
		vertices_staging_buffer_,
		physical_device_,
		MemoryPropertyFlagBits::eHostVisible
		);
	flush_vertices_to_memory();

	generate_indices_staging_buffer_allocate_info();
	initialize_buffer(device_, indices_staging_buffer_);
	indices_staging_buffer_memory_ = allocate_buffer_memory<decltype(indices_staging_buffer_memory_)::info_type>(
		device_,
		indices_staging_buffer_,
		physical_device_,
		MemoryPropertyFlagBits::eHostVisible
		);
	flush_indices_to_memory();

	generate_graphics_command_pool_create_info();
	initialize_command_pool(device_, graphics_command_pool_);

	generate_graphics_command_buffer_allocate_info();
	initialize_command_buffers(device_, graphics_command_buffers_);

	generate_sync_objects_create_info();
	for(auto& semaphore : swap_chain_image_syn_)
		initialize_semaphore(device_, semaphore);
	for(auto& semaphore : render_syn_)
		initialize_semaphore(device_, semaphore);
	for(auto& fence : gpu_syn_)
		initialize_fence(device_, fence);

	generate_render_info();
}

void vulkan_sample::re_initialize_vulkan()
{
	const auto old_extent = swap_chain_.info().info.imageExtent;
	const auto old_image_view_size = image_views_.size();
	{
		int width = 0, height = 0;
		while((width | height) == 0)
		{
			glfwGetFramebufferSize(window_, &width, &height);
			glfwWaitEvents();
		}
	}

	device_->waitIdle();

	graphics_pipeline_ = nullptr;

	pipeline_layout_ = nullptr;

	for(auto& buffer : frame_buffers_)
		buffer = nullptr;

	render_pass_ = nullptr;

	for(auto& view : image_views_)
		view = nullptr;

	swap_chain_ = nullptr;

	generate_swap_chain_create_info();
	initialize_swap_chain(device_, swap_chain_);

	generate_image_view_create_infos();
	for(auto& view : image_views_)
		initialize_image_view(device_, view);

	generate_framebuffer_create_infos();
	for(auto& fb : frame_buffers_)
		initialize_framebuffer(device_, fb);

	if(swap_chain_.info().info.imageExtent != old_extent)
	{
		generate_graphics_pipeline_create_info();
		vector<decltype(graphics_pipeline_)> graphics_pipelines_(1);
		graphics_pipelines_.front() = std::move(graphics_pipeline_);
		initialize_graphics_pipelines(device_, graphics_pipelines_);
		graphics_pipeline_ = std::move(graphics_pipelines_.front());
	}
	if(image_views_.size() != old_image_view_size)
	{
		pool_objects_reset(descriptor_sets_, device_);
		generate_descriptor_pool_create_info();
		initialize_descriptor_pool(device_, descriptor_pool_);

		generate_descriptor_set_allocate_info();
		initialize_descriptor_sets(device_, descriptor_sets_);

		pool_objects_reset(graphics_command_buffers_, device_);
		generate_graphics_command_buffer_allocate_info();

		initialize_command_buffers(device_, graphics_command_buffers_);
	}

	generate_render_info();
}

void vulkan_sample::glfw_cleanup() noexcept
{
	glfwDestroyWindow(window_);
	glfwTerminate();
	window_ = nullptr;
}

vulkan_sample::~vulkan_sample()
{
	glfw_cleanup();
}

const SwapchainCreateInfoKHR& vulkan_sample::swap_chain_create_info() const { return swap_chain_.info(); }

void vulkan_sample::initialize()
{
	initialize_window();
	initialize_vulkan();
}

bool vulkan_sample::render()
{
	static constexpr empty_type empty_type;
	return render(empty_type);
}

void vulkan_sample::flush_vertices_to_memory()
{
	write(vertices_staging_buffer_memory_, device_, vertices.cbegin(), vertices.cend());
	device_->flushMappedMemoryRanges(
		{{*vertices_staging_buffer_memory_, 0, whole_size<DeviceSize>}},
		device_.dispatch());
}

void vulkan_sample::flush_indices_to_memory()
{
	write(indices_staging_buffer_memory_, device_, indices.cbegin(), indices.cend());
	device_->flushMappedMemoryRanges({{*indices_staging_buffer_memory_, 0, whole_size<DeviceSize>}}, device_.dispatch());
}

void vulkan_sample::flush_transform_to_memory()
{
	write(transform_buffer_memory_, device_, transform);
	device_->flushMappedMemoryRanges(
		{{*transform_buffer_memory_, 0, whole_size<DeviceSize>}},
		device_.dispatch()
	);
}

void vulkan_sample::flush_to_memory()
{
	device_->flushMappedMemoryRanges({
		MappedMemoryRange{*vertices_staging_buffer_memory_, 0, whole_size<DeviceSize>},
		MappedMemoryRange{*indices_staging_buffer_memory_, 0, whole_size<DeviceSize>},
		MappedMemoryRange{*transform_buffer_memory_, 0, whole_size<DeviceSize>}
		}, device_.dispatch());
}