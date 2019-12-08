#include"info.h"

namespace vulkan::utility
{
	void info_proxy<ApplicationInfo>::property_copy(const info_proxy& right) const
	{
		application_name_property = right.application_name_property;
		engine_name_property = right.engine_name_property;
	}

	info_proxy<ApplicationInfo>::info_proxy(
		decltype(application_name_) app_name,
		decltype(engine_name_) engine_name,
		decltype(info) i
	) : base(std::move(i))
	{
		application_name_property = std::move(app_name);
		engine_name_property = std::move(engine_name);
	}

	info_proxy<ApplicationInfo>::info_proxy(base_info_type info) : info_proxy(
		info.pApplicationName != nullptr ?
		decltype(application_name_){info.pApplicationName} :
		decltype(application_name_){},
		info.pEngineName != nullptr ?
		decltype(engine_name_){info.pEngineName} :
		decltype(engine_name_){},
		std::move(info)
	)
	{}

	void info_proxy<ApplicationInfo>::set_application_name(decltype(application_name_) value)
	{
		application_name_ = std::move(value);
		info.pApplicationName = application_name_.length() != 0 ? application_name_.c_str() : nullptr;
	}

	void info_proxy<ApplicationInfo>::set_engine_name(decltype(engine_name_) value)
	{
		engine_name_ = std::move(value);
		info.pEngineName = engine_name_.length() != 0 ? engine_name_.c_str() : nullptr;
	}

	void info_proxy<InstanceCreateInfo>::property_copy(const info_proxy& right) const
	{
		application_info_property = right.application_info_property;
		extension_name_strs_property = right.extension_name_strs_property;
		layer_name_strs_property = right.layer_name_strs_property;
	}

	info_proxy<InstanceCreateInfo>::info_proxy(
		decltype(application_info_) application_info,
		decltype(extension_name_strs_) extension_name_strs,
		decltype(layer_name_strs_) layer_names_strs,
		decltype(info) i
	) : base(std::move(i))
	{
		application_info_property = std::move(application_info);
		extension_name_strs_property = std::move(extension_name_strs);
		layer_name_strs_property = std::move(layer_names_strs);
	}

	info_proxy<InstanceCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pApplicationInfo != nullptr ? std::make_optional(*info.pApplicationInfo) : nullopt},
		{info.ppEnabledExtensionNames, info.ppEnabledExtensionNames + info.enabledExtensionCount},
		{info.ppEnabledLayerNames, info.ppEnabledLayerNames + info.enabledLayerCount},
		std::move(info)
	)
	{}

	void info_proxy<InstanceCreateInfo>::set_application_info(decltype(application_info_) value)
	{
		application_info_ = std::move(value);
		info.pApplicationInfo = application_info_ ? &application_info_->info : nullptr;
	}

	void info_proxy<InstanceCreateInfo>::set_extension_name_strs(vector<string> value)
	{
		extension_name_strs_ = std::move(value);
		extension_names_.resize(extension_name_strs_.size());
		std::transform(
			extension_name_strs_.cbegin(),
			extension_name_strs_.cend(),
			extension_names_.begin(),
			[](decltype(*extension_name_strs_.cbegin()) str) { return str.c_str(); }
		);
		info.ppEnabledExtensionNames = extension_names_.size() != 0 ? extension_names_.data() : nullptr;
		info.enabledExtensionCount = static_cast<uint32_t>(extension_name_strs_.size());
	}

	void info_proxy<InstanceCreateInfo>::set_layer_name_strs(decltype(layer_name_strs_) value)
	{
		layer_name_strs_ = std::move(value);
		layer_names_.resize(layer_name_strs_.size());
		std::transform(
			layer_name_strs_.cbegin(),
			layer_name_strs_.cend(),
			layer_names_.begin(),
			[](decltype(*layer_name_strs_.cbegin()) str) { return str.c_str(); }
		);
		info.ppEnabledLayerNames = layer_names_.data();
		info.enabledLayerCount = static_cast<uint32_t>(layer_name_strs_.size());
	}

	void info_proxy<DeviceQueueCreateInfo>::property_copy(const info_proxy& right) const
	{
		priorities_property = right.priorities_property;
	}

	info_proxy<DeviceQueueCreateInfo>::info_proxy(
		decltype(priorities_) priorities,
		decltype(info) i
	) : base(std::move(i))
	{
		priorities_property = std::move(priorities);
	}

	info_proxy<DeviceQueueCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pQueuePriorities, info.pQueuePriorities + info.queueCount},
		std::move(info)
	)
	{}

	void info_proxy<DeviceQueueCreateInfo>::set_priorities(decltype(priorities_) value)
	{
		priorities_ = std::move(value);
		info.pQueuePriorities = priorities_.size() != 0 ? priorities_.data() : nullptr;
		info.queueCount = static_cast<decltype(info.queueCount)>(priorities_.size());
	}

	void info_proxy<DeviceCreateInfo>::property_copy(const info_proxy& right) const
	{
		queue_create_infos_set_property = right.queue_create_infos_set_;
		extension_name_strs_property = right.extension_name_strs_;
		features_property = right.features_;
	}

	info_proxy<DeviceCreateInfo>::info_proxy(
		decltype(queue_create_infos_set_) queue_create_infos_set,
		decltype(extension_name_strs_) extension_name_strs,
		decltype(features_) features,
		decltype(info) i
	) : base(std::move(i))
	{
		queue_create_infos_set_property = std::move(queue_create_infos_set);
		extension_name_strs_property = std::move(extension_name_strs);
		features_property = std::move(features);
	}

	info_proxy<DeviceCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pQueueCreateInfos, info.pQueueCreateInfos + info.queueCreateInfoCount},
		{info.ppEnabledExtensionNames, info.ppEnabledExtensionNames + info.enabledExtensionCount},
		{info.pEnabledFeatures != nullptr ? std::make_optional(*info.pEnabledFeatures) : nullopt},
		std::move(info)
	)
	{}

	void info_proxy<DeviceCreateInfo>::set_queue_create_info_set(decltype(queue_create_infos_set_) value)
	{
		queue_create_infos_set_ = std::move(value);
		queue_create_infos_ = {queue_create_infos_set_.cbegin(), queue_create_infos_set_.cend()};
		info.pQueueCreateInfos = queue_create_infos_.size() != 0 ? queue_create_infos_.data() : nullptr;
		info.queueCreateInfoCount = static_cast<decltype(info.queueCreateInfoCount)>(queue_create_infos_.size());
	}

	void info_proxy<DeviceCreateInfo>::set_extension_names(decltype(extension_name_strs_) value)
	{
		extension_name_strs_ = std::move(value);
		extension_names_.resize(extension_name_strs_.size());
		std::transform(
			extension_name_strs_.cbegin(),
			extension_name_strs_.cend(),
			extension_names_.begin(),
			[](decltype(*extension_name_strs_.cbegin()) name) { return name.c_str(); }
		);
		info.ppEnabledExtensionNames = extension_name_strs_.size() != 0 ? extension_names_.data() : nullptr;
		info.enabledExtensionCount = static_cast<decltype(info.enabledExtensionCount)>(extension_names_.size());
	}

	void info_proxy<DeviceCreateInfo>::set_features(decltype(features_) value)
	{
		features_ = std::move(value);
		info.pEnabledFeatures = features_ ? &*features_ : nullptr;
	}

	void info_proxy<SwapchainCreateInfoKHR>::property_copy(const info_proxy& right) const
	{
		queue_family_indices_set_property = right.queue_family_indices_set_property;
	}

	info_proxy<SwapchainCreateInfoKHR>::info_proxy(
		decltype(queue_family_indices_set_) queue_family_indices_set,
		decltype(info) i
	) : base(std::move(i))
	{
		queue_family_indices_set_property = std::move(queue_family_indices_set);
	}

	info_proxy<SwapchainCreateInfoKHR>::info_proxy(base_info_type info) : info_proxy(
		{info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount},
		std::move(info)
	)
	{}

	void info_proxy<SwapchainCreateInfoKHR>::set_queue_family_indices_set(
		decltype(queue_family_indices_set_) value
	)
	{
		queue_family_indices_set_ = std::move(value);
		queue_family_indices_ = {queue_family_indices_set_.cbegin(), queue_family_indices_set_.cend()};

		info.pQueueFamilyIndices = info.imageSharingMode == SharingMode::eConcurrent &&
			queue_family_indices_.size() != 0 ? queue_family_indices_.data() : nullptr;
		info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices_.size());
	}

	void info_proxy<SubpassDescription>::property_copy(const info_proxy& right) const
	{
		input_attachments_property = right.input_attachments_property;
		color_attachments_property = right.color_attachments_property;
		resolve_attachments_property = right.resolve_attachments_property;
		depth_attachment_property = right.depth_attachment_property;
		preserve_attachments_property = right.preserve_attachments_property;
	}

	info_proxy<SubpassDescription>::info_proxy(
		decltype(input_attachments_) input_attachments,
		decltype(color_attachments_) color_attachments,
		decltype(resolve_attachments_) resolve_attachments,
		decltype(depth_attachment_) depth_attachment,
		decltype(preserve_attachments_) preserve_attachments,
		decltype(info) i
	) : base(std::move(i))
	{
		input_attachments_property = std::move(input_attachments);
		color_attachments_property = std::move(color_attachments);
		resolve_attachments_property = std::move(resolve_attachments);
		depth_attachment_property = std::move(depth_attachment);
		preserve_attachments_property = std::move(preserve_attachments);
	}

	info_proxy<SubpassDescription>::info_proxy(base_info_type info) :
		info_proxy(
			{info.pInputAttachments, info.pInputAttachments + info.inputAttachmentCount},
			{info.pColorAttachments, info.pColorAttachments + info.colorAttachmentCount},
			{info.pResolveAttachments, info.pResolveAttachments + info.colorAttachmentCount},
			{info.pDepthStencilAttachment != nullptr ? std::make_optional(*info.pDepthStencilAttachment) : nullopt},
			{info.pPreserveAttachments, info.pPreserveAttachments + info.preserveAttachmentCount},
			std::move(info)
		)
	{}

	void info_proxy<SubpassDescription>::set_input_attachments(decltype(input_attachments_) value)
	{
		input_attachments_ = std::move(value);
		info.pInputAttachments = input_attachments_.size() != 0 ? input_attachments_.data() : nullptr;
		info.inputAttachmentCount = static_cast<uint32_t>(input_attachments_.size());
	}

	void info_proxy<SubpassDescription>::set_color_attachments(decltype(color_attachments_) value)
	{
		color_attachments_ = std::move(value);
		info.pColorAttachments = color_attachments_.size() != 0 ? color_attachments_.data() : nullptr;
		info.colorAttachmentCount = static_cast<uint32_t>(color_attachments_.size());
	}

	void info_proxy<SubpassDescription>::set_resolve_attachments(decltype(resolve_attachments_) value)
	{
		resolve_attachments_ = std::move(value);
		info.pResolveAttachments = resolve_attachments_.size() != 0 ? resolve_attachments_.data() : nullptr;
	}

	void info_proxy<SubpassDescription>::set_depth_attachment(decltype(depth_attachment_) value)
	{
		depth_attachment_ = std::move(value);
		info.pDepthStencilAttachment = depth_attachment_ ? &*depth_attachment_ : nullptr;
	}

	void info_proxy<SubpassDescription>::set_preserve_attachments(decltype(preserve_attachments_) value)
	{
		preserve_attachments_ = std::move(value);
		info.pPreserveAttachments = preserve_attachments_.size() != 0 ? preserve_attachments_.data() : nullptr;
		info.preserveAttachmentCount = static_cast<uint32_t>(preserve_attachments_.size());
	}

	void info_proxy<RenderPassCreateInfo>::property_copy(const info_proxy& right) const
	{
		attachment_descriptions_property = right.attachment_descriptions_property;
		subpass_descriptions_info_proxies_property = right.subpass_descriptions_info_proxies_property;
		dependencies_property = right.dependencies_property;
	}

	info_proxy<RenderPassCreateInfo>::info_proxy(
		decltype(attachment_descriptions_) attachment_descriptions,
		decltype(subpass_descriptions_info_proxies_) subpass_descriptions_info_proxies,
		decltype(dependencies_) dependencies,
		decltype(info) i
	) : base(std::move(i))
	{
		attachment_descriptions_property = std::move(attachment_descriptions);
		subpass_descriptions_info_proxies_property = std::move(subpass_descriptions_info_proxies);
		dependencies_property = std::move(dependencies);
	}

	info_proxy<RenderPassCreateInfo>::info_proxy(base_info_type info) :
		info_proxy(
			{info.pAttachments, info.pAttachments + info.attachmentCount},
			{info.pSubpasses, info.pSubpasses + info.subpassCount},
			{info.pDependencies, info.pDependencies + info.dependencyCount},
			std::move(info)
		)
	{}

	void info_proxy<RenderPassCreateInfo>::set_attachment_descriptions(
		decltype(attachment_descriptions_) value
	)
	{
		attachment_descriptions_ = std::move(value);
		info.pAttachments = attachment_descriptions_.size() != 0 ? attachment_descriptions_.data() : nullptr;
		info.attachmentCount = static_cast<uint32_t>(attachment_descriptions_.size());
	}

	void info_proxy<RenderPassCreateInfo>::set_subpass_descriptions_info_proxies(
		decltype(subpass_descriptions_info_proxies_) value
	)
	{
		subpass_descriptions_info_proxies_ = std::move(value);
		subpass_descriptions_.resize(subpass_descriptions_info_proxies_.size());
		std::transform(
			subpass_descriptions_info_proxies_.cbegin(),
			subpass_descriptions_info_proxies_.cend(),
			subpass_descriptions_.begin(),
			[](decltype(*subpass_descriptions_info_proxies_.cbegin()) subpass_info_proxy)	->
			decltype(subpass_descriptions_)::value_type{return subpass_info_proxy; }
		);
		info.pSubpasses = subpass_descriptions_.size() != 0 ? subpass_descriptions_.data() : nullptr;
		info.subpassCount = static_cast<uint32_t>(subpass_descriptions_.size());
	}

	void info_proxy<RenderPassCreateInfo>::set_dependencies(decltype(dependencies_) value)
	{
		dependencies_ = std::move(value);
		info.pDependencies = dependencies_.size() != 0 ? dependencies_.data() : nullptr;
		info.dependencyCount = static_cast<uint32_t>(dependencies_.size());
	}

	void info_proxy<FramebufferCreateInfo>::property_copy(const info_proxy& right) const
	{
		image_views_property = right.image_views_property;
	}

	info_proxy<FramebufferCreateInfo>::info_proxy(decltype(image_views_) views, decltype(info) i) :
		base(std::move(i))
	{
		image_views_property = std::move(views);
	}

	info_proxy<FramebufferCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pAttachments, info.pAttachments + info.attachmentCount},
		std::move(info)
	)
	{}

	void info_proxy<FramebufferCreateInfo>::set_image_views(decltype(image_views_) value)
	{
		image_views_ = std::move(value);
		info.pAttachments = image_views_.size() != 0 ? image_views_.data() : nullptr;
		info.attachmentCount = static_cast<uint32_t>(image_views_.size());
	}

	void info_proxy<ShaderModuleCreateInfo>::property_copy(const info_proxy& right) const
	{
		codes_property = right.codes_property;
	}

	info_proxy<ShaderModuleCreateInfo>::info_proxy(decltype(codes_) codes, decltype(info) info) :
		base(std::move(info))
	{
		codes_property = std::move(codes);
	}

	info_proxy<ShaderModuleCreateInfo>::info_proxy(base_info_type info) :info_proxy(
		{info.pCode, info.pCode + info.codeSize / sizeof(decltype(codes_)::value_type)},
		std::move(info)
	)
	{}

	void info_proxy<ShaderModuleCreateInfo>::set_codes(decltype(codes_) value)
	{
		codes_ = std::move(value);
		info.pCode = codes_.size() != 0 ? codes_.data() : nullptr;
		info.codeSize = codes_.size() * sizeof(decltype(codes_)::value_type);
	}

	void info_proxy<ImageCreateInfo>::property_copy(const info_proxy& right) const
	{
		queue_family_indices_set_property = right.queue_family_indices_set_property;
	}

	info_proxy<ImageCreateInfo>::info_proxy(
		decltype(queue_family_indices_set_) queue_family_indices_set,
		decltype(info) i
	) : base(std::move(i))
	{
		queue_family_indices_set_property = std::move(queue_family_indices_set);
	}

	info_proxy<ImageCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount},
		std::move(info)
	)
	{}

	void info_proxy<ImageCreateInfo>::set_queue_family_indices_set(
		decltype(queue_family_indices_set_) value
	)
	{
		queue_family_indices_set_ = std::move(value);
		queue_family_indices_ = {queue_family_indices_set_.cbegin(), queue_family_indices_set_.cend()};

		info.pQueueFamilyIndices = info.sharingMode == SharingMode::eConcurrent &&
			queue_family_indices_.size() != 0 ? queue_family_indices_.data() : nullptr;
		info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices_.size());
	}

	void info_proxy<SpecializationInfo>::property_copy(const info_proxy& right) const
	{
		map_entries_property = right.map_entries_property;
	}

	info_proxy<SpecializationInfo>::info_proxy(
		decltype(map_entries_) map_entries,
		decltype(info) i
	) : base(std::move(i))
	{
		map_entries_property = std::move(map_entries);
	}

	info_proxy<SpecializationInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pMapEntries, info.pMapEntries + info.mapEntryCount},
		std::move(info)
	)
	{}

	void info_proxy<SpecializationInfo>::set_map_entries(decltype(map_entries_) value)
	{
		map_entries_ = std::move(value);
		info.pMapEntries = map_entries_.size() != 0 ? map_entries_.data() : nullptr;
		info.mapEntryCount = static_cast<uint32_t>(map_entries_.size());
	}

	void info_proxy<PipelineShaderStageCreateInfo>::property_copy(const info_proxy& right) const
	{
		entry_name_property = right.entry_name_property;
		specialization_info_property = right.specialization_info_property;
	}

	info_proxy<PipelineShaderStageCreateInfo>::info_proxy(
		decltype(entry_name_) entry_name,
		decltype(specialization_info_) specialization_info,
		decltype(info) i
	) : base(std::move(i))
	{
		entry_name_property = std::move(entry_name);
		specialization_info_property = std::move(specialization_info);
	}

	info_proxy<PipelineShaderStageCreateInfo>::info_proxy(base_info_type info) :
		info_proxy(
			info.pName,
			{info.pSpecializationInfo != nullptr ? std::make_optional(*info.pSpecializationInfo) : nullopt},
			std::move(info)
		)
	{}

	void info_proxy<PipelineShaderStageCreateInfo>::set_entry_name(decltype(entry_name_) value)
	{
		entry_name_ = std::move(value);
		info.pName = entry_name_.length() != 0 ? entry_name_.c_str() : nullptr;
	}

	void info_proxy<PipelineShaderStageCreateInfo>::set_specialization_info(
		decltype(specialization_info_) value
	)
	{
		specialization_info_ = std::move(value);
		info.pSpecializationInfo = specialization_info_ ? &specialization_info_->info : nullptr;
	}

	void info_proxy<PipelineVertexInputStateCreateInfo>::property_copy(const info_proxy& right) const
	{
		vertex_input_binding_descriptions_property = right.vertex_input_binding_descriptions_property;
		vertex_input_attribute_descriptions_property = right.vertex_input_attribute_descriptions_property;
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::info_proxy(
		decltype(vertex_input_binding_descriptions_) vertex_input_binding_descriptions,
		decltype(vertex_input_attribute_descriptions_) vertex_input_attribute_descriptions,
		decltype(info) i
	) : base(std::move(i))
	{
		vertex_input_binding_descriptions_property = std::move(vertex_input_binding_descriptions);
		vertex_input_attribute_descriptions_property = std::move(vertex_input_attribute_descriptions);
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{
			info.pVertexBindingDescriptions,
			info.pVertexBindingDescriptions + info.vertexBindingDescriptionCount
		},
		{
			info.pVertexAttributeDescriptions,
			info.pVertexAttributeDescriptions + info.vertexAttributeDescriptionCount
		},
		std::move(info))
	{}

	void info_proxy<PipelineVertexInputStateCreateInfo>::set_vertex_input_binding_descriptions(
		decltype(vertex_input_binding_descriptions_) value
	)
	{
		vertex_input_binding_descriptions_ = std::move(value);
		info.pVertexBindingDescriptions = vertex_input_binding_descriptions_.size() != 0 ?
			vertex_input_binding_descriptions_.data() :
			nullptr;
		info.vertexBindingDescriptionCount = static_cast<uint32_t>(vertex_input_binding_descriptions_.size());
	}

	void info_proxy<PipelineVertexInputStateCreateInfo>::set_vertex_input_attribute_descriptions(
		decltype(vertex_input_attribute_descriptions_) value
	)
	{
		vertex_input_attribute_descriptions_ = std::move(value);
		info.pVertexAttributeDescriptions = vertex_input_attribute_descriptions_.size() != 0 ?
			vertex_input_attribute_descriptions_.data() :
			nullptr;
		info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_input_attribute_descriptions_.size());
	}

	void info_proxy<PipelineViewportStateCreateInfo>::property_copy(const info_proxy& right) const

	{
		viewports_property = right.viewports_property;
		scissors_property = right.scissors_property;
	}

	info_proxy<PipelineViewportStateCreateInfo>::info_proxy(
		decltype(viewports_) viewports,
		decltype(scissors_) scissors,
		decltype(info) i
	) : base(std::move(i))
	{
		viewports_property = std::move(viewports);
		scissors_property = std::move(scissors);
	}

	info_proxy<PipelineViewportStateCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pViewports, info.pViewports + info.viewportCount},
		{info.pScissors, info.pScissors + info.scissorCount},
		std::move(info)
	)
	{}

	void info_proxy<PipelineViewportStateCreateInfo>::set_viewports(decltype(viewports_) value)
	{
		viewports_ = std::move(value);
		info.pViewports = viewports_.size() != 0 ? viewports_.data() : nullptr;
		info.viewportCount = static_cast<uint32_t>(viewports_.size());
	}

	void info_proxy<PipelineViewportStateCreateInfo>::set_scissors(decltype(scissors_) value)
	{
		scissors_ = std::move(value);
		info.pScissors = scissors_.size() != 0 ? scissors_.data() : nullptr;
		info.scissorCount = static_cast<uint32_t>(scissors_.size());
	}

	void info_proxy<PipelineColorBlendStateCreateInfo>::property_copy(const info_proxy& right) const
	{
		attachment_states_property = right.attachment_states_property;
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::info_proxy(
		decltype(attachment_states_) attachment_states,
		decltype(info) i
	) : base(std::move(i))
	{
		attachment_states_property = std::move(attachment_states);
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pAttachments, info.pAttachments + info.attachmentCount},
		std::move(info)
	)
	{}

	void info_proxy<PipelineColorBlendStateCreateInfo>::set_attachment_states(
		decltype(attachment_states_) value
	)
	{
		attachment_states_ = std::move(value);
		info.pAttachments = attachment_states_.size() != 0 ? attachment_states_.data() : nullptr;
		info.attachmentCount = static_cast<decltype(info.attachmentCount)>(attachment_states_.size());
	}

	void info_proxy<PipelineLayoutCreateInfo>::property_copy(const info_proxy& right) const
	{
		set_layouts_property = right.set_layouts_property;
		push_const_ranges_property = right.push_const_ranges_property;
	}

	info_proxy<PipelineLayoutCreateInfo>::info_proxy(
		decltype(set_layouts_) set_layouts,
		decltype(push_const_ranges_) push_const_ranges,
		decltype(info) i
	) : base(std::move(i))
	{
		set_layouts_property = std::move(set_layouts);
		push_const_ranges_property = std::move(push_const_ranges);
	}

	info_proxy<PipelineLayoutCreateInfo>::info_proxy(base_info_type info) :info_proxy(
		{info.pSetLayouts, info.pSetLayouts + info.setLayoutCount},
		{info.pPushConstantRanges, info.pPushConstantRanges + info.pushConstantRangeCount},
		std::move(info)
	)
	{}

	void info_proxy<PipelineLayoutCreateInfo>::set_set_layouts(decltype(set_layouts_) value)
	{
		set_layouts_ = std::move(value);
		info.pSetLayouts = set_layouts_.size() != 0 ? set_layouts_.data() : nullptr;
		info.setLayoutCount = static_cast<uint32_t>(set_layouts_.size());
	}

	void info_proxy<PipelineLayoutCreateInfo>::set_push_const_ranges(decltype(push_const_ranges_) value)
	{
		push_const_ranges_ = std::move(value);
		info.pPushConstantRanges = push_const_ranges_.size() != 0 ? push_const_ranges_.data() : nullptr;
		info.pushConstantRangeCount = static_cast<uint32_t>(push_const_ranges_.size());
	}

	void info_proxy<graphics_pipeline_create_info>::property_copy(const info_proxy& right) const
	{
		stages_info_proxies_property = right.stages_info_proxies_property;
		vertex_input_state_property = right.vertex_input_state_property;
		input_assembly_state_property = right.input_assembly_state_property;
		tessellation_state_property = right.tessellation_state_property;
		viewport_state_property = right.viewport_state_property;
		rasterization_state_property = right.rasterization_state_property;
		multi_sample_state_property = right.multi_sample_state_property;
		depth_stencil_state_property = right.depth_stencil_state_property;
		color_blend_state_property = right.color_blend_state_property;
		dynamic_state_property = right.dynamic_state_property;
	}

	info_proxy<graphics_pipeline_create_info>::info_proxy(
		decltype(stage_info_proxies_) stages_info_proxies,
		decltype(vertex_input_state_) vertex_input_state,
		decltype(input_assembly_state_) input_assembly_state,
		decltype(tessellation_state_) tessellation_state,
		decltype(viewport_state_) viewport_state,
		decltype(rasterization_state_) rasterization_state,
		decltype(multi_sample_state_) multi_sample_state,
		decltype(depth_stencil_state_) depth_stencil_state,
		decltype(color_blend_state_) color_blend_state,
		decltype(dynamic_state_) dynamic_state,
		decltype(info) info
	) : base(std::move(info))
	{
		stages_info_proxies_property = std::move(stages_info_proxies);
		vertex_input_state_property = std::move(vertex_input_state);
		input_assembly_state_property = std::move(input_assembly_state);
		tessellation_state_property = std::move(tessellation_state);
		viewport_state_property = std::move(viewport_state);
		rasterization_state_property = std::move(rasterization_state);
		multi_sample_state_property = std::move(multi_sample_state);
		depth_stencil_state_property = std::move(depth_stencil_state);
		color_blend_state_property = std::move(color_blend_state);
		dynamic_state_property = std::move(dynamic_state);
	}

	info_proxy<graphics_pipeline_create_info>::info_proxy(base_info_type info) :info_proxy(
		{info.pStages, info.pStages + info.stageCount},
		{info.pVertexInputState != nullptr ? std::make_optional(*info.pVertexInputState) : nullopt},
		{info.pInputAssemblyState != nullptr ? std::make_optional(*info.pInputAssemblyState) : nullopt},
		{info.pTessellationState != nullptr ? std::make_optional(*info.pTessellationState) : nullopt},
		{info.pViewportState != nullptr ? std::make_optional(*info.pViewportState) : nullopt},
		{info.pRasterizationState != nullptr ? std::make_optional(*info.pRasterizationState) : nullopt},
		{info.pMultisampleState != nullptr ? std::make_optional(*info.pMultisampleState) : nullopt},
		{info.pDepthStencilState != nullptr ? std::make_optional(*info.pDepthStencilState) : nullopt},
		{info.pColorBlendState != nullptr ? std::make_optional(*info.pColorBlendState) : nullopt},
		{info.pDynamicState != nullptr ? std::make_optional(*info.pDynamicState) : nullopt},
		std::move(info)
	)
	{}

	void info_proxy<graphics_pipeline_create_info>::set_stage_info_proxies(
		decltype(stage_info_proxies_) value
	)
	{
		stage_info_proxies_ = std::move(value);
		stages_.resize(stage_info_proxies_.size());
		std::transform(
			stage_info_proxies_.cbegin(),
			stage_info_proxies_.cend(),
			stages_.begin(),
			[](decltype(*stage_info_proxies_.cbegin()) stages_info_proxy) -> decltype(stages_)::value_type{return stages_info_proxy;}
		);
		info.pStages = stages_.size() != 0 ? stages_.data() : nullptr;
		info.stageCount = static_cast<uint32_t>(stages_.size());
	}

	void info_proxy<graphics_pipeline_create_info>::set_vertex_input_state(
		decltype(vertex_input_state_) value
	)
	{
		vertex_input_state_ = std::move(value);
		info.pVertexInputState = vertex_input_state_ ? &vertex_input_state_->info : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_input_assembly_state(
		decltype(input_assembly_state_) value
	)
	{
		input_assembly_state_ = std::move(value);
		info.pInputAssemblyState = input_assembly_state_ ? &*input_assembly_state_ : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_tessellation_state(
		decltype(tessellation_state_) value
	)
	{
		tessellation_state_ = std::move(value);
		info.pTessellationState = tessellation_state_ ? &*tessellation_state_ : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_viewport_state(decltype(viewport_state_) value)
	{
		viewport_state_ = std::move(value);
		info.pViewportState = viewport_state_ ? &viewport_state_->info : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_rasterization_state(
		decltype(rasterization_state_) value
	)
	{
		rasterization_state_ = std::move(value);
		info.pRasterizationState = rasterization_state_ ? &*rasterization_state_ : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_multi_sample_state(decltype(multi_sample_state_) value)
	{
		multi_sample_state_ = std::move(value);
		info.pMultisampleState = multi_sample_state_ ? &*multi_sample_state_ : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_depth_stencil_state(
		decltype(depth_stencil_state_) value
	)
	{
		depth_stencil_state_ = std::move(value);
		info.pDepthStencilState = depth_stencil_state_ ? &*depth_stencil_state_ : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_color_blend_state(decltype(color_blend_state_) value)
	{
		color_blend_state_ = std::move(value);
		info.pColorBlendState = color_blend_state_ ? &color_blend_state_->info : nullptr;
	}

	void info_proxy<graphics_pipeline_create_info>::set_dynamic_state(decltype(dynamic_state_) value)
	{
		dynamic_state_ = std::move(value);
		info.pDynamicState = dynamic_state_ ? &*dynamic_state_ : nullptr;
	}

	void info_proxy<CommandBufferBeginInfo>::property_copy(const info_proxy& right) const
	{
		inheritance_info_property = right.inheritance_info_property;
	}

	info_proxy<CommandBufferBeginInfo>::info_proxy(
		decltype(inheritance_info_) inheritance_info,
		decltype(info) i
	) : base(std::move(i))
	{
		inheritance_info_property = std::move(inheritance_info);
	}

	info_proxy<CommandBufferBeginInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pInheritanceInfo != nullptr ? std::make_optional(*info.pInheritanceInfo) : nullopt},
		std::move(info)
	)
	{}

	void info_proxy<CommandBufferBeginInfo>::set_inheritance_info(decltype(inheritance_info_) value)
	{
		inheritance_info_ = std::move(value);
		info.pInheritanceInfo = inheritance_info_ ? &*inheritance_info_ : nullptr;
	}

	void info_proxy<RenderPassBeginInfo>::property_copy(const info_proxy& right) const
	{
		clear_values_property = right.clear_values_property;
	}

	info_proxy<RenderPassBeginInfo>::info_proxy(
		decltype(clear_values_) clear_values,
		decltype(info) i
	) : base(std::move(i))
	{
		clear_values_property = std::move(clear_values);
	}

	info_proxy<RenderPassBeginInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pClearValues, info.pClearValues + info.clearValueCount},
		std::move(info)
	)
	{}

	void info_proxy<RenderPassBeginInfo>::set_clear_values(decltype(clear_values_) value)
	{
		clear_values_ = std::move(value);
		info.pClearValues = clear_values_.size() != 0 ? clear_values_.data() : nullptr;
		info.clearValueCount = static_cast<decltype(info.clearValueCount)>(clear_values_.size());
	}

	void info_proxy<DescriptorSetLayoutBinding>::property_copy(const info_proxy& right) const
	{
		samplers_property = right.samplers_property;
	}

	info_proxy<DescriptorSetLayoutBinding>::info_proxy(decltype(samplers_) samplers, decltype(info) i) :
		base(std::move(i))
	{
		samplers_property = std::move(samplers);
	}

	info_proxy<DescriptorSetLayoutBinding>::info_proxy(base_info_type info) :info_proxy(
		info.pImmutableSamplers != nullptr ?
		decltype(samplers_){info.pImmutableSamplers, info.pImmutableSamplers + info.descriptorCount} :
		decltype(samplers_){0},
		std::move(info)
	)
	{}

	void info_proxy<DescriptorSetLayoutBinding>::set_samplers(decltype(samplers_) value)
	{
		samplers_ = std::move(value);
		info.pImmutableSamplers = info.descriptorType == DescriptorType::eSampledImage ?
			samplers_.data() :
			nullptr;
	}

	void info_proxy<DescriptorSetLayoutCreateInfo>::property_copy(const info_proxy& right) const
	{
		bindings_proxies_property = right.bindings_proxies_property;
	}

	info_proxy<DescriptorSetLayoutCreateInfo>::info_proxy(
		decltype(bindings_proxies_) bindings_proxies,
		decltype(info) i
	) : base(std::move(i))
	{
		bindings_proxies_property = std::move(bindings_proxies);
	}

	info_proxy<DescriptorSetLayoutCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pBindings, info.pBindings + info.bindingCount},
		std::move(info)
	)
	{}

	void info_proxy<DescriptorSetLayoutCreateInfo>::set_binding_proxies(decltype(bindings_proxies_) value)
	{
		bindings_proxies_ = std::move(value);
		bindings_.resize(bindings_proxies_.size());
		std::transform(
			bindings_proxies_.cbegin(),
			bindings_proxies_.cend(),
			bindings_.begin(),
			[](decltype(*bindings_proxies_.cbegin()) binding) -> decltype(bindings_)::value_type{return binding; }
		);
		info.pBindings = bindings_proxies_.size() != 0 ? bindings_.data() : nullptr;
		info.bindingCount = static_cast<decltype(info.bindingCount)>(bindings_proxies_.size());
	}

	void info_proxy<DescriptorPoolCreateInfo>::property_copy(const info_proxy& right) const
	{
		pool_sizes_property = right.pool_sizes_property;
	}

	info_proxy<DescriptorPoolCreateInfo>::info_proxy(
		decltype(pool_sizes_) pool_sizes,
		decltype(info) i
	) : base(std::move(i))
	{
		pool_sizes_property = std::move(pool_sizes);
	}

	info_proxy<DescriptorPoolCreateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pPoolSizes, info.pPoolSizes + info.poolSizeCount},
		std::move(info)
	)
	{}

	void info_proxy<DescriptorPoolCreateInfo>::set_pool_sizes(decltype(pool_sizes_) value)
	{
		pool_sizes_ = std::move(value);
		info.pPoolSizes = pool_sizes_.size() != 0 ? pool_sizes_.data() : nullptr;
		info.poolSizeCount = static_cast<decltype(info.poolSizeCount)>(pool_sizes_.size());
		info.maxSets = 0;
		for(const auto& pool_size : pool_sizes_)
			info.maxSets += pool_size.type != DescriptorType::eInlineUniformBlockEXT ? pool_size.descriptorCount : 0;
	}

	void info_proxy<DescriptorSetAllocateInfo>::property_copy(const info_proxy& right) const
	{
		descriptor_set_layouts_property = right.descriptor_set_layouts_property;
	}

	info_proxy<DescriptorSetAllocateInfo>::info_proxy(
		decltype(descriptor_set_layouts_) descriptor_set_layouts,
		decltype(info) i
	) : base(std::move(i))
	{
		descriptor_set_layouts_property = std::move(descriptor_set_layouts);
	}

	info_proxy<DescriptorSetAllocateInfo>::info_proxy(base_info_type info) : info_proxy(
		{info.pSetLayouts, info.pSetLayouts + info.descriptorSetCount},
		std::move(info)
	)
	{}

	void info_proxy<DescriptorSetAllocateInfo>::set_descriptor_set_layouts(
		decltype(descriptor_set_layouts_) value
	)
	{
		descriptor_set_layouts_ = std::move(value);
		info.pSetLayouts = descriptor_set_layouts_.size() != 0 ? descriptor_set_layouts_.data() : nullptr;
		info.descriptorSetCount = static_cast<uint32_t>(descriptor_set_layouts_.size());
	}

	void info_proxy<WriteDescriptorSet>::property_copy(const info_proxy& right) const
	{
		image_infos_property = right.image_infos_property;
		buffer_infos_property = right.buffer_infos_property;
		texel_buffer_views_property = right.texel_buffer_views_property;
	}

	info_proxy<WriteDescriptorSet>::info_proxy(
		decltype(image_infos_) image_infos,
		decltype(buffer_infos_) buffer_infos,
		decltype(texel_buffer_views_) texel_buffer_views,
		decltype(info) i
	) :base(std::move(i))
	{
		image_infos_property = std::move(image_infos);
		buffer_infos_property = std::move(buffer_infos);
		texel_buffer_views_property = std::move(texel_buffer_views);
	}

	info_proxy<WriteDescriptorSet>::info_proxy(base_info_type info) :info_proxy(
		{info.pImageInfo, info.pImageInfo},
		{info.pBufferInfo, info.pBufferInfo},
		{info.pTexelBufferView, info.pTexelBufferView},
		std::move(info)
	)
	{}

	void info_proxy<WriteDescriptorSet>::set_image_infos(decltype(image_infos_) value)
	{
		image_infos_ = std::move(value);
		info.pImageInfo = image_infos_.size() != 0 ? image_infos_.data() : nullptr;
	}

	void info_proxy<WriteDescriptorSet>::set_buffer_infos(decltype(buffer_infos_) value)
	{
		buffer_infos_ = std::move(value);
		info.pBufferInfo = buffer_infos_.size() != 0 ? buffer_infos_.data() : nullptr;
	}

	void info_proxy<WriteDescriptorSet>::set_texel_buffer_views(decltype(texel_buffer_views_) value)
	{
		texel_buffer_views_ = std::move(value);
		info.pTexelBufferView = texel_buffer_views_.size() != 0 ? texel_buffer_views_.data() : nullptr;
	}

	void info_proxy<SubmitInfo>::property_copy(const info_proxy& right) const
	{
		wait_semaphores_property = right.wait_semaphores_property;
		dst_stage_mask_property = right.dst_stage_mask_property;
		command_buffers_property = right.command_buffers_property;
		signal_semaphores_property = right.signal_semaphores_property;
	}

	info_proxy<SubmitInfo>::info_proxy(
		decltype(wait_semaphores_) wait_semaphores,
		decltype(dst_stage_mask_) dst_stage_mask,
		decltype(command_buffers_) command_buffers,
		decltype(signal_semaphores_) signal_semaphores,
		decltype(info) info
	) : base(std::move(info))
	{
		wait_semaphores_property = std::move(wait_semaphores);
		dst_stage_mask_property = std::move(dst_stage_mask);
		command_buffers_property = std::move(command_buffers);
		signal_semaphores_property = std::move(signal_semaphores);
	}

	info_proxy<SubmitInfo>::info_proxy(base_info_type info) :info_proxy(
		{info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount},
		{info.pWaitDstStageMask != nullptr ? std::make_optional(*info.pWaitDstStageMask) : nullopt},
		{info.pCommandBuffers, info.pCommandBuffers + info.commandBufferCount},
		{info.pSignalSemaphores, info.pSignalSemaphores + info.signalSemaphoreCount},
		std::move(info)
	)
	{}

	void info_proxy<SubmitInfo>::set_wait_semaphores(decltype(wait_semaphores_) value)
	{
		wait_semaphores_ = std::move(value);
		info.pWaitSemaphores = wait_semaphores_.size() != 0 ? wait_semaphores_.data() : nullptr;
		info.waitSemaphoreCount = static_cast<decltype(info.waitSemaphoreCount)>(wait_semaphores_.size());
	}

	void info_proxy<SubmitInfo>::set_dst_stage_mask(decltype(dst_stage_mask_) value)
	{
		dst_stage_mask_ = std::move(value);
		info.pWaitDstStageMask = dst_stage_mask_ ? &*dst_stage_mask_ : nullptr;
	}

	void info_proxy<SubmitInfo>::set_command_buffers(decltype(command_buffers_) value)
	{
		command_buffers_ = std::move(value);
		info.pCommandBuffers = command_buffers_.size() != 0 ? command_buffers_.data() : nullptr;
		info.commandBufferCount = static_cast<uint32_t>(command_buffers_.size());
	}

	void info_proxy<SubmitInfo>::set_signal_semaphores(decltype(signal_semaphores_) value)
	{
		signal_semaphores_ = std::move(value);
		info.pSignalSemaphores = signal_semaphores_.size() != 0 ? signal_semaphores_.data() : nullptr;
		info.signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores_.size());
	}

	void info_proxy<PresentInfoKHR>::property_copy(const info_proxy& right) const
	{
		wait_semaphores_property = right.wait_semaphores_property;
		swapchains_property = right.swapchains_property;
		image_indices_property = right.image_indices_property;
		results_property = right.results_property;
	}

	info_proxy<PresentInfoKHR>::info_proxy(
		decltype(wait_semaphores_) wait_semaphores,
		decltype(swapchains_) swapchains,
		decltype(image_indices_) image_indices,
		decltype(results_) results,
		decltype(info) i
	) : base(std::move(i))
	{
		wait_semaphores_property = std::move(wait_semaphores);
		swapchains_property = std::move(swapchains);
		image_indices_property = std::move(image_indices);
		results_property = std::move(results);
	}

	info_proxy<PresentInfoKHR>::info_proxy(base_info_type info) :info_proxy(
		{info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount},
		{info.pSwapchains, info.pSwapchains + info.swapchainCount},
		{info.pImageIndices, info.pImageIndices + info.swapchainCount},
		{info.pResults, info.pResults + info.swapchainCount},
		std::move(info)
	)
	{}

	void info_proxy<PresentInfoKHR>::set_wait_semaphores(decltype(wait_semaphores_) value)
	{
		wait_semaphores_ = std::move(value);
		info.pWaitSemaphores = wait_semaphores_.size() != 0 ? wait_semaphores_.data() : nullptr;
		info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores_.size());
	}

	void info_proxy<PresentInfoKHR>::set_swapchains(decltype(swapchains_) value)
	{
		swapchains_ = std::move(value);
		info.pSwapchains = swapchains_.size() != 0 ? swapchains_.data() : nullptr;
		info.swapchainCount = static_cast<uint32_t>(swapchains_.size());
	}

	void info_proxy<PresentInfoKHR>::set_image_indices(decltype(image_indices_) value)
	{
		image_indices_ = std::move(value);
		info.pImageIndices = image_indices_.size() ? image_indices_.data() : nullptr;
	}

	void info_proxy<PresentInfoKHR>::set_results(decltype(results_) value)
	{
		results_ = std::move(value);
		info.pResults = results_.size() != 0 ? results_.data() : nullptr;
	}

	void info_proxy<BufferCreateInfo>::property_copy(const info_proxy& right) const
	{
		queue_family_indices_set_property = right.queue_family_indices_set_property;
	}

	info_proxy<BufferCreateInfo>::info_proxy(decltype(queue_family_indices_set_) q_indices, decltype(info) info) :
		base(std::move(info))
	{
		queue_family_indices_set_property = std::move(q_indices);
	}

	info_proxy<BufferCreateInfo>::info_proxy(base_info_type info) :info_proxy(
		{info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount},
		std::move(info)
	)
	{}

	void info_proxy<BufferCreateInfo>::set_queue_family_indices_set(decltype(queue_family_indices_set_) value)
	{
		queue_family_indices_set_ = std::move(value);
		queue_family_indices_ = {queue_family_indices_set_.cbegin(), queue_family_indices_set_.cend()};
		info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices_.size());
		info.pQueueFamilyIndices = info.sharingMode == SharingMode::eConcurrent ? queue_family_indices_.data() : nullptr;
	}
}