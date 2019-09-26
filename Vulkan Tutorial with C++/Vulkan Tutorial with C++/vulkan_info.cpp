#include"vulkan_info.h"

namespace vulkan
{
	info_proxy<ApplicationInfo>::info_proxy(
		decltype(application_name) app_name,
		decltype(engine_name) engine_n,
		decltype(info) i
	) :
		info(std::move(i)),
		application_name(std::move(app_name)),
		engine_name(std::move(engine_n))
	{
		set_proxy();
	}

	info_proxy<ApplicationInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		application_name(info.pApplicationName),
		engine_name(info.pEngineName)
	{
		set_proxy();
	}

	info_proxy<ApplicationInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		application_name(right.application_name),
		engine_name(right.engine_name)
	{
		set_proxy();
	}

	info_proxy<ApplicationInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		application_name(std::move(right.application_name)),
		engine_name(std::move(right.engine_name))
	{
		set_proxy();
	}

	info_proxy<ApplicationInfo>& info_proxy<ApplicationInfo>::operator=(const info_proxy& right) noexcept
	{
		application_name = right.application_name;
		engine_name = right.engine_name;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<ApplicationInfo>& info_proxy<ApplicationInfo>::operator=(info_proxy&& right) noexcept
	{
		application_name = std::move(right.application_name);
		engine_name = std::move(right.engine_name);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<ApplicationInfo>::operator ApplicationInfo& () { return info; }

	info_proxy<ApplicationInfo>::operator const ApplicationInfo& () const { return info; }

	void info_proxy<ApplicationInfo>::set_proxy()
	{
		info.pApplicationName = application_name.c_str();
		info.pEngineName = engine_name.c_str();
	}

	info_proxy<InstanceCreateInfo>::info_proxy(
		decltype(application_info) a_info,
		decltype(extension_names) e_ns,
		decltype(layer_names) l_ns,
		decltype(info)  i
	) :
		info(std::move(i)),
		application_info(std::move(a_info)),
		extension_names(std::move(e_ns)),
		layer_names(std::move(l_ns))
	{
		set_proxy();
	}

	info_proxy<InstanceCreateInfo>::info_proxy(info_type info) : info(std::move(info)),
		application_info(
			info.pApplicationInfo != nullptr ?
			std::make_optional(std::move(*info.pApplicationInfo)) :
			nullopt),
		extension_names(info.ppEnabledExtensionNames, info.ppEnabledExtensionNames + info.enabledExtensionCount),
		layer_names(info.ppEnabledLayerNames, info.ppEnabledLayerNames + info.enabledLayerCount)
	{
		set_proxy();
	}

	info_proxy<InstanceCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		application_info(right.application_info),
		extension_names(right.extension_names),
		layer_names(right.layer_names)
	{
		set_proxy();
	}

	info_proxy<InstanceCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		application_info(std::move(right.application_info)),
		extension_names(std::move(right.extension_names)),
		layer_names(std::move(right.layer_names)),
		extension_names_(std::move(right.extension_names_)),
		layer_names_(std::move(right.layer_names_))
	{
		set_proxy();
	}

	info_proxy<InstanceCreateInfo>& info_proxy<InstanceCreateInfo>::operator=(const info_proxy& right)
	{
		application_info = right.application_info;
		extension_names = right.extension_names;
		layer_names = right.layer_names;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<InstanceCreateInfo>& info_proxy<InstanceCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		application_info = std::move(right.application_info);
		extension_names = std::move(right.extension_names);
		layer_names = std::move(right.layer_names);
		info = std::move(right.info);
		extension_names_ = std::move(right.extension_names_);
		layer_names_ = std::move(right.layer_names_);
		set_proxy();
		return *this;
	}

	info_proxy<InstanceCreateInfo>::operator InstanceCreateInfo& () { return info; }

	info_proxy<InstanceCreateInfo>::operator const InstanceCreateInfo& () const { return info; }

	void info_proxy<InstanceCreateInfo>::set_proxy()
	{
		info.pApplicationInfo = application_info ? &application_info->info : nullptr;

		extension_names_.resize(extension_names.size());
		std::generate(
			extension_names_.begin(),
			extension_names_.end(),
			[it = std::move(extension_names.cbegin())]()mutable { return (it++)->c_str(); }
		);
		info.ppEnabledExtensionNames = extension_names_.data();
		info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());

		layer_names_.resize(layer_names.size());
		std::generate(
			layer_names_.begin(),
			layer_names_.end(),
			[it = std::move(layer_names.cbegin())]()mutable { return (it++)->c_str(); }
		);
		info.ppEnabledLayerNames = layer_names_.data();
		info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
	}

	info_proxy<DeviceQueueCreateInfo>::
		info_proxy(decltype(priorities) p, decltype(info) i) : info(std::move(i)), priorities(std::move(p))
	{
		set_proxy();
	}

	info_proxy<DeviceQueueCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		priorities(info.pQueuePriorities, info.pQueuePriorities + info.queueCount)
	{
		set_proxy();
	}

	info_proxy<DeviceQueueCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		priorities(right.priorities)
	{
		set_proxy();
	}

	info_proxy<DeviceQueueCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		priorities(std::move(right.priorities))
	{
		set_proxy();
	}

	info_proxy<DeviceQueueCreateInfo>& info_proxy<DeviceQueueCreateInfo>::operator =(const info_proxy& right)
	{
		priorities = right.priorities;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<DeviceQueueCreateInfo>& info_proxy<DeviceQueueCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		priorities = std::move(right.priorities);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<DeviceQueueCreateInfo>::operator DeviceQueueCreateInfo& () { return info; }

	info_proxy<DeviceQueueCreateInfo>::operator const DeviceQueueCreateInfo& () const { return info; }

	void info_proxy<DeviceQueueCreateInfo>::set_proxy()
	{
		info.pQueuePriorities = priorities.data();
		info.queueCount = static_cast<uint32_t>(priorities.size());
	}

	info_proxy<DeviceCreateInfo>::info_proxy(
		decltype(queue_create_infos) q_infos,
		decltype(extension_names) e_ns,
		decltype(features) f,
		decltype(info) i
	) :
		info(std::move(i)),
		queue_create_infos(std::move(q_infos)),
		extension_names(std::move(e_ns)),
		features(std::move(f))
	{
		set_proxy();
	}

	info_proxy<DeviceCreateInfo>::info_proxy(info_type info) : info(std::move(info)),
		queue_create_infos_(info.pQueueCreateInfos, info.pQueueCreateInfos + info.queueCreateInfoCount),
		extension_names(info.ppEnabledExtensionNames, info.ppEnabledExtensionNames + info.enabledExtensionCount),
		features(std::forward<std::remove_pointer_t<decltype(info.pEnabledFeatures)>>(*info.pEnabledFeatures))
	{
		for(auto& queue_create_info : queue_create_infos_)
			queue_create_infos.insert({queue_create_info.queueFamilyIndex, queue_create_info});
		set_proxy();
	}

	info_proxy<DeviceCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		queue_create_infos(right.queue_create_infos),
		extension_names(right.extension_names),
		features(right.features)
	{
		set_proxy();
	}

	info_proxy<DeviceCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		queue_create_infos_(std::move(right.queue_create_infos_)),
		enabled_extension_names_(std::move(right.enabled_extension_names_)),
		queue_create_infos(std::move(right.queue_create_infos)),
		extension_names(std::move(right.extension_names)),
		features(std::move(right.features))
	{
		set_proxy();
	}

	info_proxy<DeviceCreateInfo>& info_proxy<DeviceCreateInfo>::operator=(const info_proxy& right)
	{
		queue_create_infos = right.queue_create_infos;
		extension_names = right.extension_names;
		features = right.features;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<DeviceCreateInfo>& info_proxy<DeviceCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		queue_create_infos = std::move(right.queue_create_infos);
		extension_names = std::move(right.extension_names);
		features = std::move(right.features);
		info = std::move(right.info);
		queue_create_infos_ = std::move(right.queue_create_infos_);
		enabled_extension_names_ = std::move(right.enabled_extension_names_);
		set_proxy();
		return *this;
	}

	info_proxy<DeviceCreateInfo>::operator DeviceCreateInfo& () { return info; }

	info_proxy<DeviceCreateInfo>::operator const DeviceCreateInfo& () const { return info; }

	void info_proxy<DeviceCreateInfo>::set_proxy()
	{
		queue_create_infos_.resize(queue_create_infos.size());
		std::generate(
			queue_create_infos_.begin(),
			queue_create_infos_.end(),
			[it = std::move(queue_create_infos.cbegin())]()mutable { return it++->second.info; }
		);
		info.pQueueCreateInfos = queue_create_infos_.data();
		info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos_.size());

		enabled_extension_names_.resize(extension_names.size());
		std::generate(
			enabled_extension_names_.begin(),
			enabled_extension_names_.end(),
			[it = std::move(extension_names.cbegin())]()mutable { return it->c_str(); }
		);
		info.ppEnabledExtensionNames = enabled_extension_names_.data();
		info.enabledExtensionCount = static_cast<uint32_t>(enabled_extension_names_.size());

		info.pEnabledFeatures = features ? &*features : nullptr;
	}

	info_proxy<SwapchainCreateInfoKHR>::info_proxy(decltype(queue_family_indices) q_indices, decltype(info) info) :
		info(std::move(info)),
		queue_family_indices(std::move(q_indices))
	{
		set_proxy();
	}

	info_proxy<SwapchainCreateInfoKHR>::info_proxy(info_type info) :
		info(std::move(info)),
		queue_family_indices(info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount)
	{
		set_proxy();
	}

	info_proxy<SwapchainCreateInfoKHR>::info_proxy(const info_proxy& right) :
		info(right.info),
		queue_family_indices(right.queue_family_indices)
	{
		set_proxy();
	}

	info_proxy<SwapchainCreateInfoKHR>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		queue_family_indices_(std::move(right.queue_family_indices_)),
		queue_family_indices(std::move(right.queue_family_indices))
	{
		set_proxy();
	}

	info_proxy<SwapchainCreateInfoKHR>& info_proxy<SwapchainCreateInfoKHR>::operator =(const info_proxy& right)
	{
		queue_family_indices = right.queue_family_indices;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<SwapchainCreateInfoKHR>& info_proxy<SwapchainCreateInfoKHR>::operator=(info_proxy&& right) noexcept
	{
		queue_family_indices = std::move(right.queue_family_indices);
		info = std::move(right.info);
		queue_family_indices_ = std::move(right.queue_family_indices_);
		set_proxy();
		return *this;
	}

	info_proxy<SwapchainCreateInfoKHR>::operator SwapchainCreateInfoKHR& () { return info; }

	info_proxy<SwapchainCreateInfoKHR>::operator const SwapchainCreateInfoKHR& () const { return info; }

	void info_proxy<SwapchainCreateInfoKHR>::set_proxy()
	{
		queue_family_indices_.resize(queue_family_indices.size());
		std::generate(
			queue_family_indices_.begin(),
			queue_family_indices_.end(),
			[it = std::move(queue_family_indices.cbegin())]()mutable { return *it++; }
		);
		info.pQueueFamilyIndices = queue_family_indices_.data();
		info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices_.size());
	}

	info_proxy<SubpassDescription>::info_proxy(
		decltype(input_attachments) i_a,
		decltype(color_attachments) c_a,
		decltype(resolve_attachments) r_a,
		decltype(depth_attachment) d_a,
		decltype(preserve_attachments) p_a,
		decltype(info) info
	) :
		info(std::move(info)),
		input_attachments(std::move(i_a)),
		color_attachments(std::move(c_a)),
		resolve_attachments(std::move(r_a)),
		depth_attachment(std::move(d_a)),
		preserve_attachments(std::move(p_a))
	{
		set_proxy();
	}

	info_proxy<SubpassDescription>::info_proxy(info_type info) : info(std::move(info)),
		input_attachments(info.pInputAttachments, info.pInputAttachments + info.inputAttachmentCount),
		color_attachments(info.pColorAttachments, info.pColorAttachments + info.colorAttachmentCount),
		resolve_attachments(info.pResolveAttachments, info.pResolveAttachments + info.colorAttachmentCount),
		depth_attachment(
			info.pDepthStencilAttachment != nullptr ?
			std::make_optional(std::move(*info.pDepthStencilAttachment)) :
			nullopt
		),
		preserve_attachments(info.pPreserveAttachments, info.pPreserveAttachments + info.preserveAttachmentCount)
	{
		set_proxy();
	}

	info_proxy<SubpassDescription>::info_proxy(const info_proxy& right) :
		info(right.info),
		input_attachments(right.input_attachments),
		color_attachments(right.color_attachments),
		resolve_attachments(right.resolve_attachments),
		depth_attachment(right.depth_attachment),
		preserve_attachments(right.preserve_attachments)
	{
		set_proxy();
	}

	info_proxy<SubpassDescription>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		input_attachments(std::move(right.input_attachments)),
		color_attachments(std::move(right.color_attachments)),
		resolve_attachments(std::move(right.resolve_attachments)),
		depth_attachment(std::move(right.depth_attachment)),
		preserve_attachments(std::move(right.preserve_attachments))
	{
		set_proxy();
	}

	info_proxy<SubpassDescription>& info_proxy<SubpassDescription>::operator=(const info_proxy& right)
	{
		input_attachments = right.input_attachments;
		color_attachments = right.color_attachments;
		resolve_attachments = right.resolve_attachments;
		depth_attachment = right.depth_attachment;
		preserve_attachments = right.preserve_attachments;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<SubpassDescription>& info_proxy<SubpassDescription>::operator=(info_proxy&& right) noexcept
	{
		input_attachments = std::move(right.input_attachments);
		color_attachments = std::move(right.color_attachments);
		resolve_attachments = std::move(right.resolve_attachments);
		depth_attachment = std::move(right.depth_attachment);
		preserve_attachments = std::move(right.preserve_attachments);
		info = right.info;
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<SubpassDescription>::operator SubpassDescription& () { return info; }

	info_proxy<SubpassDescription>::operator const SubpassDescription& () const { return info; }

	void info_proxy<SubpassDescription>::set_proxy()
	{
		info.pInputAttachments = input_attachments.data();
		info.inputAttachmentCount = static_cast<uint32_t>(input_attachments.size());

		info.pColorAttachments = color_attachments.data();
		info.colorAttachmentCount = static_cast<uint32_t>(color_attachments.size());

		info.pResolveAttachments = resolve_attachments.data();

		info.pDepthStencilAttachment = depth_attachment ? &*depth_attachment : nullptr;

		info.pPreserveAttachments = preserve_attachments.data();
		info.preserveAttachmentCount = static_cast<uint32_t>(preserve_attachments.size());
	}

	info_proxy<RenderPassCreateInfo>::info_proxy(
		decltype(attachment_descriptions) a_descriptions,
		decltype(subpass_descriptions) s_descriptions,
		decltype(dependencies) dps,
		decltype(info) info
	) :
		info(std::move(info)),
		attachment_descriptions(std::move(a_descriptions)),
		subpass_descriptions(std::move(s_descriptions)),
		dependencies(std::move(dps))
	{
		set_proxy();
	}

	info_proxy<RenderPassCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		attachment_descriptions(info.pAttachments, info.pAttachments + info.attachmentCount),
		subpass_descriptions(info.pSubpasses, info.pSubpasses + info.subpassCount),
		dependencies(info.pDependencies, info.pDependencies + info.dependencyCount)
	{
		set_proxy();
	}

	info_proxy<RenderPassCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		attachment_descriptions(right.attachment_descriptions),
		subpass_descriptions(right.subpass_descriptions),
		dependencies(right.dependencies)
	{
		set_proxy();
	}

	info_proxy<RenderPassCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		attachment_descriptions(std::move(right.attachment_descriptions)),
		subpass_descriptions(std::move(right.subpass_descriptions)),
		dependencies(std::move(right.dependencies))
	{
		set_proxy();
	}

	info_proxy<RenderPassCreateInfo>& info_proxy<RenderPassCreateInfo>::operator=(const info_proxy& right)
	{
		attachment_descriptions = right.attachment_descriptions;
		subpass_descriptions = right.subpass_descriptions;
		dependencies = right.dependencies;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<RenderPassCreateInfo>& info_proxy<RenderPassCreateInfo>::operator =(info_proxy&& right) noexcept
	{
		attachment_descriptions = std::move(right.attachment_descriptions);
		subpass_descriptions = std::move(right.subpass_descriptions);
		dependencies = std::move(right.dependencies);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<RenderPassCreateInfo>::operator RenderPassCreateInfo& () { return info; }

	info_proxy<RenderPassCreateInfo>::operator const RenderPassCreateInfo& () const { return info; }

	void info_proxy<RenderPassCreateInfo>::set_proxy()
	{
		info.pAttachments = attachment_descriptions.data();
		info.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size());

		subpass_descriptions_.resize(subpass_descriptions.size());
		std::generate(
			subpass_descriptions_.begin(),
			subpass_descriptions_.end(),
			[&, i = size_t(0)]()mutable { return subpass_descriptions[i++].info; }
		);
		info.pSubpasses = subpass_descriptions_.data();
		info.subpassCount = static_cast<uint32_t>(subpass_descriptions_.size());

		info.pDependencies = dependencies.data();
		info.dependencyCount = static_cast<uint32_t>(dependencies.size());
	}

	info_proxy<FramebufferCreateInfo>::info_proxy(decltype(image_views) p, decltype(info) i) :
		info(std::move(i)),
		image_views(std::move(p))
	{
		set_proxy();
	}

	info_proxy<FramebufferCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		image_views(info.pAttachments, info.pAttachments + info.attachmentCount)
	{
		set_proxy();
	}

	info_proxy<FramebufferCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		image_views(right.image_views)
	{
		set_proxy();
	}

	info_proxy<FramebufferCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		image_views(std::move(right.image_views))
	{
		set_proxy();
	}

	info_proxy<FramebufferCreateInfo>& info_proxy<FramebufferCreateInfo>::operator =(const info_proxy& right)
	{
		image_views = right.image_views;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<FramebufferCreateInfo>& info_proxy<FramebufferCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		image_views = std::move(right.image_views);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<FramebufferCreateInfo>::operator FramebufferCreateInfo& () { return info; }

	info_proxy<FramebufferCreateInfo>::operator const FramebufferCreateInfo& () const { return info; }

	void info_proxy<FramebufferCreateInfo>::set_proxy()
	{
		info.pAttachments = image_views.data();
		info.attachmentCount = static_cast<uint32_t>(image_views.size());
	}

	info_proxy<ShaderModuleCreateInfo>::info_proxy(decltype(codes) c_p, decltype(info) info) :
		info(std::move(info)),
		codes(std::move(c_p))
	{
		set_proxy();
	}

	info_proxy<ShaderModuleCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		codes(info.pCode, info.pCode + info.codeSize / sizeof(decltype(codes)::value_type))
	{
		set_proxy();
	}

	info_proxy<ShaderModuleCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		codes(right.codes)
	{
		set_proxy();
	}

	info_proxy<ShaderModuleCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		codes(std::move(right.codes))
	{
		set_proxy();
	}

	info_proxy<ShaderModuleCreateInfo>& info_proxy<ShaderModuleCreateInfo>::operator =(const info_proxy& right)
	{
		codes = right.codes;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<ShaderModuleCreateInfo>& info_proxy<ShaderModuleCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		codes = std::move(right.codes);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<ShaderModuleCreateInfo>::operator ShaderModuleCreateInfo& () { return info; }

	info_proxy<ShaderModuleCreateInfo>::operator const ShaderModuleCreateInfo& () const { return info; }

	void info_proxy<ShaderModuleCreateInfo>::set_proxy()
	{
		info.pCode = codes.data();
		info.codeSize = codes.size() * sizeof(decltype(codes)::value_type);
	}

	info_proxy<SpecializationInfo>::info_proxy(decltype(map_entries) p, decltype(info) i) :
		info(std::move(i)),
		map_entries(std::move(p))
	{
		set_proxy();
	}

	info_proxy<SpecializationInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		map_entries(info.pMapEntries, info.pMapEntries + info.mapEntryCount)
	{
		set_proxy();
	}

	info_proxy<SpecializationInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		map_entries(right.map_entries)
	{
		set_proxy();
	}

	info_proxy<SpecializationInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		map_entries(std::move(right.map_entries))
	{
		set_proxy();
	}

	info_proxy<SpecializationInfo>& info_proxy<SpecializationInfo>::operator=(const info_proxy& right)
	{
		map_entries = right.map_entries;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<SpecializationInfo>& info_proxy<SpecializationInfo>::operator=(info_proxy&& right) noexcept
	{
		map_entries = std::move(right.map_entries);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<SpecializationInfo>::operator SpecializationInfo& () { return info; }

	info_proxy<SpecializationInfo>::operator const SpecializationInfo& () const { return info; }

	void info_proxy<SpecializationInfo>::set_proxy()
	{
		info.pMapEntries = map_entries.data();
		info.mapEntryCount = static_cast<uint32_t>(map_entries.size());
	}

	info_proxy<PipelineShaderStageCreateInfo>::info_proxy(
		decltype(entry_name) n,
		decltype(specialization_info) p,
		decltype(info) i
	) :
		info(std::move(i)),
		entry_name(std::move(n)),
		specialization_info(std::move(p))
	{
		set_proxy();
	}

	info_proxy<PipelineShaderStageCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		entry_name(info.pName),
		specialization_info(
			info.pSpecializationInfo != nullptr ?
			std::make_optional(std::move(*info.pSpecializationInfo)) :
			nullopt
		)
	{
		set_proxy();
	}

	info_proxy<PipelineShaderStageCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		entry_name(right.entry_name),
		specialization_info(right.specialization_info)
	{
		set_proxy();
	}

	info_proxy<PipelineShaderStageCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		entry_name(std::move(right.entry_name)),
		specialization_info(std::move(right.specialization_info))
	{
		set_proxy();
	}

	info_proxy<PipelineShaderStageCreateInfo>&
		info_proxy<PipelineShaderStageCreateInfo>::operator=(const info_proxy& right)
	{
		entry_name = right.entry_name;
		specialization_info = right.specialization_info;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<PipelineShaderStageCreateInfo>&
		info_proxy<PipelineShaderStageCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		entry_name = std::move(right.entry_name);
		specialization_info = std::move(right.specialization_info);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<PipelineShaderStageCreateInfo>::operator PipelineShaderStageCreateInfo& () { return info; }

	info_proxy<PipelineShaderStageCreateInfo>::operator const PipelineShaderStageCreateInfo& () const
	{
		return info;
	}

	void info_proxy<PipelineShaderStageCreateInfo>::set_proxy()
	{
		info.pName = entry_name.c_str();
		info.pSpecializationInfo = specialization_info ? &specialization_info->info : nullptr;
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::info_proxy(
		decltype(vertex_input_binding_descriptions) binding,
		decltype(vertex_input_attribute_descriptions) attribute,
		decltype(info) i
	) :
		info(std::move(i)),
		vertex_input_binding_descriptions(std::move(binding)),
		vertex_input_attribute_descriptions(std::move(attribute))
	{
		set_proxy();
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		vertex_input_binding_descriptions(
			info.pVertexBindingDescriptions,
			info.pVertexBindingDescriptions + info.vertexBindingDescriptionCount
		),
		vertex_input_attribute_descriptions(
			info.pVertexAttributeDescriptions,
			info.pVertexAttributeDescriptions + info.vertexAttributeDescriptionCount
		)
	{
		set_proxy();
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		vertex_input_binding_descriptions(right.vertex_input_binding_descriptions),
		vertex_input_attribute_descriptions(right.vertex_input_attribute_descriptions)
	{
		set_proxy();
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		vertex_input_binding_descriptions(std::move(right.vertex_input_binding_descriptions)),
		vertex_input_attribute_descriptions(std::move(right.vertex_input_attribute_descriptions))
	{
		set_proxy();
	}

	info_proxy<PipelineVertexInputStateCreateInfo>&
		info_proxy<PipelineVertexInputStateCreateInfo>::operator=(const info_proxy& right)
	{
		vertex_input_attribute_descriptions = right.vertex_input_attribute_descriptions;
		vertex_input_binding_descriptions = right.vertex_input_binding_descriptions;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<PipelineVertexInputStateCreateInfo>&
		info_proxy<PipelineVertexInputStateCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		vertex_input_attribute_descriptions = std::move(right.vertex_input_attribute_descriptions);
		vertex_input_binding_descriptions = std::move(right.vertex_input_binding_descriptions);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::operator PipelineVertexInputStateCreateInfo& ()
	{
		return info;
	}

	info_proxy<PipelineVertexInputStateCreateInfo>::operator
		const PipelineVertexInputStateCreateInfo& () const { return info; }

	void info_proxy<PipelineVertexInputStateCreateInfo>::set_proxy()
	{
		info.pVertexAttributeDescriptions = vertex_input_attribute_descriptions.data();
		info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_input_attribute_descriptions.size());

		info.pVertexBindingDescriptions = vertex_input_binding_descriptions.data();
		info.vertexBindingDescriptionCount = static_cast<uint32_t>(vertex_input_binding_descriptions.size());
	}

	info_proxy<PipelineViewportStateCreateInfo>::info_proxy(
		decltype(viewports) v_ps,
		decltype(scissors) ss,
		decltype(info) i
	) :
		info(std::move(i)), viewports(std::move(v_ps)), scissors(std::move(ss))
	{
		set_proxy();
	}

	info_proxy<PipelineViewportStateCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		viewports(info.pViewports, info.pViewports + info.viewportCount),
		scissors(info.pScissors, info.pScissors + info.scissorCount)
	{
		set_proxy();
	}

	info_proxy<PipelineViewportStateCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		viewports(right.viewports),
		scissors(right.scissors)
	{
		set_proxy();
	}

	info_proxy<PipelineViewportStateCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		viewports(std::move(right.viewports)),
		scissors(std::move(right.scissors))
	{
		set_proxy();
	}

	info_proxy<PipelineViewportStateCreateInfo>&
		info_proxy<PipelineViewportStateCreateInfo>::operator=(const info_proxy& right)
	{
		viewports = right.viewports;
		scissors = right.scissors;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<PipelineViewportStateCreateInfo>&
		info_proxy<PipelineViewportStateCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		viewports = std::move(right.viewports);
		scissors = std::move(right.scissors);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<PipelineViewportStateCreateInfo>::operator PipelineViewportStateCreateInfo& () { return info; }

	info_proxy<PipelineViewportStateCreateInfo>::operator const PipelineViewportStateCreateInfo& () const
	{
		return info;
	}

	void info_proxy<PipelineViewportStateCreateInfo>::set_proxy()
	{
		info.pViewports = viewports.data();
		info.viewportCount = static_cast<uint32_t>(viewports.size());

		info.pScissors = scissors.data();
		info.scissorCount = static_cast<uint32_t>(scissors.size());
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::info_proxy(decltype(attachment_states) a_ss, decltype(info) i) :
		info(std::move(i)),
		attachment_states(std::move(a_ss))
	{
		set_proxy();
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		attachment_states(info.pAttachments, info.pAttachments + info.attachmentCount)
	{
		set_proxy();
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		attachment_states(right.attachment_states)
	{
		set_proxy();
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		attachment_states(std::move(right.attachment_states))
	{
		set_proxy();
	}

	info_proxy<PipelineColorBlendStateCreateInfo>&
		info_proxy<PipelineColorBlendStateCreateInfo>::operator=(const info_proxy& right)
	{
		attachment_states = right.attachment_states;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<PipelineColorBlendStateCreateInfo>&
		info_proxy<PipelineColorBlendStateCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		attachment_states = std::move(right.attachment_states);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::operator PipelineColorBlendStateCreateInfo& ()
	{
		return info;
	}

	info_proxy<PipelineColorBlendStateCreateInfo>::operator const PipelineColorBlendStateCreateInfo& () const
	{
		return info;
	}

	void info_proxy<PipelineColorBlendStateCreateInfo>::set_proxy()
	{
		info.pAttachments = attachment_states.data();
		info.attachmentCount = static_cast<uint32_t>(attachment_states.size());
	}

	info_proxy<PipelineLayoutCreateInfo>::info_proxy(
		decltype(set_layouts) layouts,
		decltype(push_const_ranges) ranges,
		decltype(info) i
	) :
		info(std::move(i)),
		set_layouts(std::move(layouts)),
		push_const_ranges(std::move(ranges))
	{
		set_proxy();
	}

	info_proxy<PipelineLayoutCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		set_layouts(info.pSetLayouts, info.pSetLayouts + info.setLayoutCount),
		push_const_ranges(info.pPushConstantRanges, info.pPushConstantRanges + info.pushConstantRangeCount)
	{
		set_proxy();
	}

	info_proxy<PipelineLayoutCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		set_layouts(right.set_layouts),
		push_const_ranges(right.push_const_ranges)
	{
		set_proxy();
	}

	info_proxy<PipelineLayoutCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		set_layouts(std::move(right.set_layouts)),
		push_const_ranges(std::move(right.push_const_ranges))
	{
		set_proxy();
	}

	info_proxy<PipelineLayoutCreateInfo>&
		info_proxy<PipelineLayoutCreateInfo>::operator=(const info_proxy& right)
	{
		set_layouts = right.set_layouts;
		push_const_ranges = right.push_const_ranges;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<PipelineLayoutCreateInfo>&
		info_proxy<PipelineLayoutCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		set_layouts = std::move(right.set_layouts);
		push_const_ranges = std::move(right.push_const_ranges);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<PipelineLayoutCreateInfo>::operator PipelineLayoutCreateInfo& () { return info; }

	info_proxy<PipelineLayoutCreateInfo>::operator const PipelineLayoutCreateInfo& () const { return info; }

	void info_proxy<PipelineLayoutCreateInfo>::set_proxy()
	{
		info.pSetLayouts = set_layouts.data();
		info.setLayoutCount = static_cast<uint32_t>(set_layouts.size());

		info.pPushConstantRanges = push_const_ranges.data();
		info.pushConstantRangeCount = static_cast<uint32_t>(push_const_ranges.size());
	}

	info_proxy<GraphicsPipelineCreateInfo>::info_proxy(
		decltype(stages_create_infos) stages,
		decltype(vertex_input_state) vertex,
		decltype(input_assembly_state) input,
		decltype(tessellation_state) tessellation,
		decltype(viewport_state) viewport,
		decltype(rasterization_state_create_info) rasterization,
		decltype(multi_sample_state_create_info) multi_sample,
		decltype(depth_stencil_state_create_info) depth_stencil,
		decltype(color_blend_state_create_info) color_blend,
		decltype(dynamic_state_create_info) dynamic_state,
		decltype(info) info
	) :
		info(std::move(info)),
		stages_create_infos(std::move(stages)),
		vertex_input_state(std::move(vertex)),
		input_assembly_state(std::move(input)),
		tessellation_state(std::move(tessellation)),
		viewport_state(std::move(viewport)),
		rasterization_state_create_info(std::move(rasterization)),
		multi_sample_state_create_info(std::move(multi_sample)),
		depth_stencil_state_create_info(std::move(depth_stencil)),
		color_blend_state_create_info(std::move(color_blend)),
		dynamic_state_create_info(std::move(dynamic_state))
	{
		set_proxy();
	}

	info_proxy<GraphicsPipelineCreateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		stages_create_infos(info.pStages, info.pStages + info.stageCount),
		vertex_input_state(
			info.pVertexInputState != nullptr ?
			std::make_optional(std::move(*info.pVertexInputState)) :
			nullopt
		),
		input_assembly_state(
			info.pInputAssemblyState != nullptr ?
			std::make_optional(std::move(*info.pInputAssemblyState)) :
			nullopt
		),
		tessellation_state(
			info.pTessellationState != nullptr ?
			std::make_optional(
				std::move(*info.pTessellationState)) :
			nullopt
		),
		viewport_state(
			info.pViewportState != nullptr ?
			std::make_optional(std::move(*info.pViewportState)) :
			nullopt
		),
		rasterization_state_create_info(
			info.pRasterizationState != nullptr ?
			std::make_optional(std::move(*info.pRasterizationState)) :
			nullopt
		),
		multi_sample_state_create_info(
			info.pMultisampleState != nullptr ?
			std::make_optional(std::move(*info.pMultisampleState)) :
			nullopt
		),
		depth_stencil_state_create_info(
			info.pDepthStencilState != nullptr ?
			std::make_optional(std::move(*info.pDepthStencilState)) :
			nullopt
		),
		color_blend_state_create_info(
			info.pColorBlendState != nullptr ?
			std::make_optional(std::move(*info.pColorBlendState)) :
			nullopt
		),
		dynamic_state_create_info(
			info.pDynamicState != nullptr ?
			std::make_optional(std::move(*info.pDynamicState)) :
			nullopt
		)
	{
		set_proxy();
	}

	info_proxy<GraphicsPipelineCreateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		stages_create_infos(right.stages_create_infos),
		vertex_input_state(right.vertex_input_state),
		input_assembly_state(right.input_assembly_state),
		tessellation_state(right.tessellation_state),
		viewport_state(right.viewport_state),
		rasterization_state_create_info(right.rasterization_state_create_info),
		multi_sample_state_create_info(right.multi_sample_state_create_info),
		depth_stencil_state_create_info(right.depth_stencil_state_create_info),
		color_blend_state_create_info(right.color_blend_state_create_info),
		dynamic_state_create_info(right.dynamic_state_create_info)
	{
		set_proxy();
	}

	info_proxy<GraphicsPipelineCreateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		stages_create_infos(std::move(right.stages_create_infos)),
		vertex_input_state(std::move(right.vertex_input_state)),
		input_assembly_state(std::move(right.input_assembly_state)),
		tessellation_state(std::move(right.tessellation_state)),
		viewport_state(std::move(right.viewport_state)),
		rasterization_state_create_info(std::move(right.rasterization_state_create_info)),
		multi_sample_state_create_info(std::move(right.multi_sample_state_create_info)),
		depth_stencil_state_create_info(std::move(right.depth_stencil_state_create_info)),
		color_blend_state_create_info(std::move(right.color_blend_state_create_info)),
		dynamic_state_create_info(std::move(right.dynamic_state_create_info))
	{
		set_proxy();
	}

	info_proxy<GraphicsPipelineCreateInfo>&
		info_proxy<GraphicsPipelineCreateInfo>::operator=(const info_proxy& right)
	{
		stages_create_infos = right.stages_create_infos;
		vertex_input_state = right.vertex_input_state;
		input_assembly_state = right.input_assembly_state;
		tessellation_state = right.tessellation_state;
		viewport_state = right.viewport_state;
		rasterization_state_create_info = right.rasterization_state_create_info;
		multi_sample_state_create_info = right.multi_sample_state_create_info;
		depth_stencil_state_create_info = right.depth_stencil_state_create_info;
		color_blend_state_create_info = right.color_blend_state_create_info;
		dynamic_state_create_info = right.dynamic_state_create_info;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<GraphicsPipelineCreateInfo>&
		info_proxy<GraphicsPipelineCreateInfo>::operator=(info_proxy&& right) noexcept
	{
		stages_create_infos = std::move(right.stages_create_infos);
		vertex_input_state = std::move(right.vertex_input_state);
		input_assembly_state = std::move(right.input_assembly_state);
		tessellation_state = std::move(right.tessellation_state);
		viewport_state = std::move(right.viewport_state);
		rasterization_state_create_info = std::move(right.rasterization_state_create_info);
		multi_sample_state_create_info = std::move(right.multi_sample_state_create_info);
		depth_stencil_state_create_info = std::move(right.depth_stencil_state_create_info);
		color_blend_state_create_info = std::move(right.color_blend_state_create_info);
		dynamic_state_create_info = std::move(right.dynamic_state_create_info);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<GraphicsPipelineCreateInfo>::operator GraphicsPipelineCreateInfo& () { return info; }

	info_proxy<GraphicsPipelineCreateInfo>::operator const GraphicsPipelineCreateInfo& () const { return info; }

	void info_proxy<GraphicsPipelineCreateInfo>::set_proxy()
	{
		stages_.resize(stages_create_infos.size());
		std::generate(
			stages_.begin(),
			stages_.end(),
			[&, it = std::move(stages_create_infos.cbegin())]()mutable { return it++->info; }
		);
		info.pStages = stages_.data();
		info.stageCount = static_cast<uint32_t>(stages_.size());

		info.pVertexInputState = vertex_input_state ? &vertex_input_state->info : nullptr;

		info.pInputAssemblyState = input_assembly_state ? &*input_assembly_state : nullptr;

		info.pTessellationState = tessellation_state ? &*tessellation_state : nullptr;

		info.pViewportState = viewport_state ? &viewport_state->info : nullptr;

		info.pRasterizationState = rasterization_state_create_info ? &*rasterization_state_create_info : nullptr;

		info.pMultisampleState = multi_sample_state_create_info ? &*multi_sample_state_create_info : nullptr;

		info.pDepthStencilState = depth_stencil_state_create_info ? &*depth_stencil_state_create_info : nullptr;

		info.pColorBlendState = color_blend_state_create_info ? &color_blend_state_create_info->info : nullptr;

		info.pDynamicState = dynamic_state_create_info ? &*dynamic_state_create_info : nullptr;
	}

	info_proxy<CommandBufferBeginInfo>::info_proxy(decltype(inheritance_info) i_i, decltype(info) i) :
		info(std::move(i)),
		inheritance_info(std::move(i_i))
	{
		set_proxy();
	}

	info_proxy<CommandBufferBeginInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		inheritance_info(
			info.pInheritanceInfo != nullptr ?
			std::make_optional(std::move((*info.pInheritanceInfo))) :
			nullopt
		)
	{
		set_proxy();
	}

	info_proxy<CommandBufferBeginInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		inheritance_info(right.inheritance_info)
	{
		set_proxy();
	}

	info_proxy<CommandBufferBeginInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		inheritance_info(std::move(right.inheritance_info))
	{
		set_proxy();
	}

	info_proxy<CommandBufferBeginInfo>& info_proxy<CommandBufferBeginInfo>::operator =(const info_proxy& right)
	{
		inheritance_info = right.inheritance_info;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<CommandBufferBeginInfo>& info_proxy<CommandBufferBeginInfo>::operator=(info_proxy&& right) noexcept
	{
		inheritance_info = std::move(right.inheritance_info);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<CommandBufferBeginInfo>::operator CommandBufferBeginInfo& () { return info; }

	info_proxy<CommandBufferBeginInfo>::operator const CommandBufferBeginInfo& () const { return info; }

	void info_proxy<CommandBufferBeginInfo>::set_proxy()
	{
		info.pInheritanceInfo = inheritance_info ? &*inheritance_info : nullptr;
	}

	info_proxy<RenderPassBeginInfo>::info_proxy(decltype(clear_values) c_vs, decltype(info) i) :
		info(std::move(i)),
		clear_values(std::move(c_vs))
	{
		set_proxy();
	}

	info_proxy<RenderPassBeginInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		clear_values(info.pClearValues, info.pClearValues + info.clearValueCount)
	{
		set_proxy();
	}

	info_proxy<RenderPassBeginInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		clear_values(right.clear_values)
	{
		set_proxy();
	}

	info_proxy<RenderPassBeginInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		clear_values(std::move(right.clear_values))
	{
		set_proxy();
	}

	info_proxy<RenderPassBeginInfo>& info_proxy<RenderPassBeginInfo>::operator=(const info_proxy& right)
	{
		clear_values = right.clear_values;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<RenderPassBeginInfo>& info_proxy<RenderPassBeginInfo>::operator =(info_proxy&& right) noexcept
	{
		clear_values = std::move(right.clear_values);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<RenderPassBeginInfo>::operator RenderPassBeginInfo& () { return info; }

	info_proxy<RenderPassBeginInfo>::operator const RenderPassBeginInfo& () const { return info; }

	void info_proxy<RenderPassBeginInfo>::set_proxy()
	{
		info.pClearValues = clear_values.data();
		info.clearValueCount = static_cast<uint32_t>(clear_values.size());
	}

	info_proxy<DescriptorSetAllocateInfo>::info_proxy(
		decltype(descriptor_set_layouts) layouts,
		decltype(info) i
	) :
		info(std::move(i)),
		descriptor_set_layouts(std::move(layouts))
	{
		set_proxy();
	}

	info_proxy<DescriptorSetAllocateInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		descriptor_set_layouts(info.pSetLayouts, info.pSetLayouts + info.descriptorSetCount)
	{
		set_proxy();
	}

	info_proxy<DescriptorSetAllocateInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		descriptor_set_layouts(right.descriptor_set_layouts)
	{
		set_proxy();
	}

	info_proxy<DescriptorSetAllocateInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		descriptor_set_layouts(std::move(right.descriptor_set_layouts))
	{
		set_proxy();
	}

	info_proxy<DescriptorSetAllocateInfo>&
		info_proxy<DescriptorSetAllocateInfo>::operator=(const info_proxy& right)
	{
		descriptor_set_layouts = right.descriptor_set_layouts;
		info = right.info;
		return *this;
	}

	info_proxy<DescriptorSetAllocateInfo>&
		info_proxy<DescriptorSetAllocateInfo>::operator=(info_proxy&& right) noexcept
	{
		descriptor_set_layouts = std::move(right.descriptor_set_layouts);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<DescriptorSetAllocateInfo>::operator DescriptorSetAllocateInfo& () { return info; }

	info_proxy<DescriptorSetAllocateInfo>::operator const DescriptorSetAllocateInfo& () const { return info; }

	void info_proxy<DescriptorSetAllocateInfo>::set_proxy()
	{
		info.pSetLayouts = descriptor_set_layouts.data();
		info.descriptorSetCount = static_cast<uint32_t>(descriptor_set_layouts.size());
	}

	info_proxy<SubmitInfo>::info_proxy(
		decltype(wait_semaphores) w_ss,
		decltype(dst_stage_mask) d_s_ms,
		decltype(command_buffers) c_bs,
		decltype(signal_semaphores) s_ss,
		decltype(info) info
	) :
		info(std::move(info)),
		wait_semaphores(std::move(w_ss)),
		dst_stage_mask(std::move(d_s_ms)),
		command_buffers(std::move(c_bs)),
		signal_semaphores(std::move(s_ss))
	{
		set_proxy();
	}

	info_proxy<SubmitInfo>::info_proxy(info_type info) :
		info(std::move(info)),
		wait_semaphores(info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount),
		dst_stage_mask(
			info.pWaitDstStageMask != nullptr ?
			std::make_optional(std::move(*info.pWaitDstStageMask)) :
			nullopt
		),
		command_buffers(info.pCommandBuffers, info.pCommandBuffers + info.commandBufferCount),
		signal_semaphores(info.pSignalSemaphores, info.pSignalSemaphores + info.signalSemaphoreCount)
	{
		set_proxy();
	}

	info_proxy<SubmitInfo>::info_proxy(const info_proxy& right) :
		info(right.info),
		wait_semaphores(right.wait_semaphores),
		dst_stage_mask(right.dst_stage_mask),
		command_buffers(right.command_buffers),
		signal_semaphores(right.signal_semaphores)
	{
		set_proxy();
	}

	info_proxy<SubmitInfo>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		wait_semaphores(std::move(right.wait_semaphores)),
		dst_stage_mask(std::move(right.dst_stage_mask)),
		command_buffers(std::move(right.command_buffers)),
		signal_semaphores(std::move(right.signal_semaphores))
	{
		set_proxy();
	}

	info_proxy<SubmitInfo>& info_proxy<SubmitInfo>::operator=(const info_proxy& right)
	{
		wait_semaphores = right.wait_semaphores;
		dst_stage_mask = right.dst_stage_mask;
		command_buffers = right.command_buffers;
		signal_semaphores = right.signal_semaphores;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<SubmitInfo>::operator SubmitInfo& () { return info; }

	info_proxy<SubmitInfo>::operator const SubmitInfo& () const { return info; }

	info_proxy<SubmitInfo>& info_proxy<SubmitInfo>::operator=(info_proxy&& right) noexcept
	{
		wait_semaphores = std::move(right.wait_semaphores);
		dst_stage_mask = std::move(right.dst_stage_mask);
		command_buffers = std::move(right.command_buffers);
		signal_semaphores = std::move(right.signal_semaphores);
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	void info_proxy<SubmitInfo>::set_proxy()
	{
		info.pWaitSemaphores = wait_semaphores.data();
		info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores.size());

		info.pWaitDstStageMask = dst_stage_mask ? &*dst_stage_mask : nullptr;

		info.pCommandBuffers = command_buffers.data();
		info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		info.pSignalSemaphores = signal_semaphores.data();
		info.signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores.size());
	}

	info_proxy<PresentInfoKHR>::info_proxy(
		decltype(semaphores) w_ss,
		decltype(swap_chains) s_cs,
		decltype(image_indices) indices,
		decltype(results) r_ss,
		decltype(info) info
	) :
		info(std::move(info)),
		semaphores(std::move(w_ss)),
		swap_chains(std::move(s_cs)),
		image_indices(std::move(indices)),
		results(std::move(r_ss))
	{
		set_proxy();
	}

	info_proxy<PresentInfoKHR>::info_proxy(info_type info) :
		info(std::move(info)),
		semaphores(info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount),
		swap_chains(info.pSwapchains, info.pSwapchains + info.swapchainCount),
		image_indices(info.pImageIndices, info.pImageIndices + info.swapchainCount),
		results(info.pResults, info.pResults + info.swapchainCount)
	{
		set_proxy();
	}

	info_proxy<PresentInfoKHR>::info_proxy(const info_proxy& right) :
		info(right.info),
		semaphores(right.semaphores),
		swap_chains(right.swap_chains),
		image_indices(right.image_indices),
		results(right.results)
	{
		set_proxy();
	}

	info_proxy<PresentInfoKHR>::info_proxy(info_proxy&& right) noexcept :
		info(std::move(right.info)),
		semaphores(std::move(right.semaphores)),
		swap_chains(std::move(right.swap_chains)),
		image_indices(std::move(right.image_indices)),
		results(std::move(right.results))
	{
		set_proxy();
	}

	info_proxy<PresentInfoKHR>& info_proxy<PresentInfoKHR>::operator=(const info_proxy& right)
	{
		semaphores = right.semaphores;
		swap_chains = right.swap_chains;
		image_indices = right.image_indices;
		results = right.results;
		info = right.info;
		set_proxy();
		return *this;
	}

	info_proxy<PresentInfoKHR>& info_proxy<PresentInfoKHR>::operator=(info_proxy&& right) noexcept
	{
		semaphores = std::move(right.semaphores);
		swap_chains = std::move(right.swap_chains);
		image_indices = std::move(right.image_indices);
		results = std::move(right.results);
		info = right.info;
		info = std::move(right.info);
		set_proxy();
		return *this;
	}

	info_proxy<PresentInfoKHR>::operator PresentInfoKHR& () { return info; }

	info_proxy<PresentInfoKHR>::operator const PresentInfoKHR& () const { return info; }

	void info_proxy<PresentInfoKHR>::set_proxy()
	{
		info.pWaitSemaphores = semaphores.data();
		info.waitSemaphoreCount = static_cast<uint32_t>(semaphores.size());

		info.pSwapchains = swap_chains.data();
		info.swapchainCount = static_cast<uint32_t>(swap_chains.size());

		info.pImageIndices = image_indices.data();

		info.pResults = results.data();
	}
}