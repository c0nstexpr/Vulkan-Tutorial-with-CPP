#pragma once

#include"utility.h"

namespace vulkan
{
	using namespace vk;

	// ReSharper disable CppInconsistentNaming
	struct GraphicsPipeline
	{
		using handle_type = Pipeline;
	};
	struct ComputePipeline
	{
		using handle_type = Pipeline;
	};

	// ReSharper restore CppInconsistentNaming

	template<typename T>
	struct info;

	template<typename T>
	struct info_proxy;

	template<>
	struct info<DebugUtilsMessengerEXT>
	{
		using handle_type = DebugUtilsMessengerEXT;
		using type = DebugUtilsMessengerCreateInfoEXT;
	};

	template<>
	struct info<SurfaceKHR>
	{
		using handle_type = SurfaceKHR;
		using type = empty_type;
	};

	template<>
	struct info<ImageView>
	{
		using handle_type = ImageView;
		using type = ImageViewCreateInfo;
	};

	template<>
	struct info<CommandPool>
	{
		using handle_type = CommandPool;
		using type = CommandPoolCreateInfo;
	};

	template<>
	struct info<CommandBuffer>
	{
		using handle_type = CommandBuffer;
		using type = CommandBufferAllocateInfo;
	};

	template<>
	struct info<Semaphore>
	{
		using handle_type = Semaphore;
		using type = SemaphoreCreateInfo;
	};

	template<>
	struct info<Fence>
	{
		using handle_type = Fence;
		using type = FenceCreateInfo;
	};

	template<>
	struct info_proxy<ApplicationInfo>
	{
		using info_type = ApplicationInfo;

		info_type info;
		string application_name;
		string engine_name;

		template<
			typename T = decltype(application_name),
			typename U = decltype(engine_name),
			typename V = decltype(info),
			typename = enable_if_convertible_t<T, decltype(application_name)>,
			typename = enable_if_convertible_t<U, decltype(engine_name)>,
			typename = enable_if_convertible_t<V, decltype(info)>>
			explicit info_proxy(
				T && app_name = T{},
				U && engine_n = U{},
				V && i = V{}
			) :
			application_name(std::forward<T>(app_name)),
			engine_name(std::forward<U>(engine_n)),
			info(std::forward<V>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			application_name(info.pApplicationName),
			engine_name(info.pEngineName),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			application_name(right.application_name),
			engine_name(right.engine_name),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			application_name(std::move(right.application_name)),
			engine_name(std::move(right.engine_name)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right) noexcept
		{
			application_name = right.application_name;
			engine_name = right.engine_name;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			application_name = std::move(right.application_name);
			engine_name = std::move(right.engine_name);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pApplicationName = application_name.c_str();
			info.pEngineName = engine_name.c_str();
		}
	};

	template<>
	struct info_proxy<InstanceCreateInfo>
	{
		using info_type = InstanceCreateInfo;

		info_type info;
		optional<info_proxy<decay_to_origin_t<decltype(info.pApplicationInfo)>>> application_info;
		vector<string> extension_names;
		vector<string> layer_names;

		template<
			typename T = decltype(application_info),
			typename U = decltype(extension_names),
			typename V = decltype(layer_names),
			typename W = decltype(info),
			typename = enable_if_convertible_t<T, decltype(application_info)>,
			typename = enable_if_convertible_t<U, decltype(extension_names)>,
			typename = enable_if_convertible_t<V, decltype(layer_names)>,
			typename = enable_if_convertible_t<W, decltype(info)>>
			explicit info_proxy(
				T && a_info = T{},
				U && e_ns = U{},
				V && l_ns = V{},
				W && i = W{}
			) :
			application_info(std::forward<T>(a_info)),
			extension_names(std::forward<U>(e_ns)),
			layer_names(std::forward<V>(l_ns)),
			info(std::forward<W>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			application_info(make_optional(std::forward<std::remove_pointer_t<
				decltype(info.pApplicationInfo)
				>>(*info.pApplicationInfo))),
			extension_names(info.ppEnabledExtensionNames, info.ppEnabledExtensionNames + info.enabledExtensionCount),
			layer_names(info.ppEnabledLayerNames, info.ppEnabledLayerNames + info.enabledLayerCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			application_info(right.application_info),
			extension_names(right.extension_names),
			layer_names(right.layer_names),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			application_info(std::move(right.application_info)),
			extension_names(std::move(right.extension_names)),
			layer_names(std::move(right.layer_names)),
			info(std::move(right.info)),
			extension_names_(std::move(right.extension_names_)),
			layer_names_(std::move(right.layer_names_))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			application_info = right.application_info;
			extension_names = right.extension_names;
			layer_names = right.layer_names;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
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

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			extension_names_.resize(extension_names.size());
			std::generate(extension_names_.begin(), extension_names_.end(),
				[it = std::move(extension_names.cbegin())]()mutable {return (it++)->c_str(); });

			info.pApplicationInfo = application_info ? &application_info->info : nullptr;

			info.ppEnabledExtensionNames = extension_names_.data();
			info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());

			layer_names_.resize(layer_names.size());
			std::generate(layer_names_.begin(), layer_names_.end(),
				[it = std::move(layer_names.cbegin())]()mutable{return (it++)->c_str(); });

			info.ppEnabledLayerNames = layer_names_.data();
			info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
		}

	private:
		vector<const char*> extension_names_;
		vector<const char*> layer_names_;
	};

	template<>
	struct info<Instance>
	{
		using handle_type = Instance;
		using type = info_proxy<InstanceCreateInfo>;
	};

	template<>
	struct info_proxy<DeviceQueueCreateInfo>
	{
		using info_type = DeviceQueueCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pQueuePriorities)>> priorities;

		template<
			typename T = decltype(priorities),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(priorities)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& p = T{}, U&& i = U{}) :
			priorities(std::forward<T>(p)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			priorities(info.pQueuePriorities, info.pQueuePriorities + info.queueCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			priorities(right.priorities),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			priorities(std::move(right.priorities)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			priorities = right.priorities;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			priorities = std::move(right.priorities);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pQueuePriorities = priorities.data();
			info.queueCount = static_cast<uint32_t>(priorities.size());
		}
	};

	template<>
	struct info_proxy<DeviceCreateInfo>
	{
		using info_type = DeviceCreateInfo;

		info_type info;

	private:
		using queue_create_info = decay_to_origin_t<decltype(info.pQueueCreateInfos)>;

		vector<queue_create_info> queue_create_infos_;
		vector<const char*> enabled_extension_names_;

	public:

		//The "queueFamilyIndex" in "DeviceQueueCreateInfo" is required to be unique
		map<
			decltype(queue_create_infos_.front().queueFamilyIndex),
			info_proxy<queue_create_info>
		> queue_create_infos;
		vector<string> extension_names;
		optional<decay_to_origin_t<decltype(info.pEnabledFeatures)>> features;

		template<
			typename T = decltype(queue_create_infos),
			typename U = decltype(extension_names),
			typename V = decltype(features),
			typename W = decltype(info),
			typename = enable_if_convertible_t<T, decltype(queue_create_infos)>,
			typename = enable_if_convertible_t<U, decltype(extension_names)>,
			typename = enable_if_convertible_t<V, decltype(features)>,
			typename = enable_if_convertible_t<W, decltype(info)>
		>
			explicit info_proxy(
				T&& q_infos = T{},
				U&& e_ns = U{},
				V&& f = V{},
				W&& i = W{}
			) :
			queue_create_infos(std::forward<T>(q_infos)),
			extension_names(std::forward<U>(e_ns)),
			features(std::forward<V>(f)),
			info(std::forward<W>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			queue_create_infos_(info.pQueueCreateInfos, info.pQueueCreateInfos + info.queueCreateInfoCount),
			extension_names(info.ppEnabledExtensionNames, info.ppEnabledExtensionNames + info.enabledExtensionCount),
			features(std::forward<std::remove_pointer_t<decltype(info.pEnabledFeatures)>>(*info.pEnabledFeatures)),
			info(std::forward<InfoType>(info))
		{
			for(auto& queue_create_info : queue_create_infos_)
				queue_create_infos.insert({queue_create_info.queueFamilyIndex, queue_create_info});
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			queue_create_infos(right.queue_create_infos),
			extension_names(right.extension_names),
			features(right.features),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			queue_create_infos(std::move(right.queue_create_infos)),
			extension_names(std::move(right.extension_names)),
			features(std::move(right.features)),
			info(std::move(right.info)),
			queue_create_infos_(std::move(right.queue_create_infos_)),
			enabled_extension_names_(std::move(right.enabled_extension_names_))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			queue_create_infos = right.queue_create_infos;
			extension_names = right.extension_names;
			features = right.features;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
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

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			queue_create_infos_.resize(queue_create_infos.size());
			std::generate(queue_create_infos_.begin(), queue_create_infos_.end(),
				[it = std::move(queue_create_infos.cbegin())]()mutable {return it++->second.info; });

			info.pQueueCreateInfos = queue_create_infos_.data();
			info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos_.size());

			enabled_extension_names_.resize(extension_names.size());
			std::generate(enabled_extension_names_.begin(), enabled_extension_names_.end(),
				[it = std::move(extension_names.cbegin())]()mutable {return it->c_str(); });

			info.ppEnabledExtensionNames = enabled_extension_names_.data();
			info.enabledExtensionCount = static_cast<uint32_t>(enabled_extension_names_.size());

			info.pEnabledFeatures = features ? &*features : nullptr;
		}
	};

	template<>
	struct info<Device>
	{
		using handle_type = Device;
		using type = info_proxy<DeviceCreateInfo>;
	};

	template<>
	struct info_proxy<SwapchainCreateInfoKHR>
	{
		using info_type = SwapchainCreateInfoKHR;

		info_type info;

	private:
		vector<decay_to_origin_t<decltype(info.pQueueFamilyIndices)>> queue_family_indices_;

	public:
		set<std::decay_t<decltype(queue_family_indices_)::value_type>> queue_family_indices;

		template<
			typename T = decltype(queue_family_indices),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(queue_family_indices)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& q_indices = T{}, U&& info = U{}) :
			queue_family_indices(std::forward<T>(q_indices)),
			info(std::forward<U>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			queue_family_indices(info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			queue_family_indices(right.queue_family_indices),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			queue_family_indices(std::move(right.queue_family_indices)),
			info(std::move(right.info)),
			queue_family_indices_(std::move(right.queue_family_indices_))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			queue_family_indices = right.queue_family_indices;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			queue_family_indices = std::move(right.queue_family_indices);
			info = std::move(right.info);
			queue_family_indices_ = std::move(right.queue_family_indices_);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			queue_family_indices_.resize(queue_family_indices.size());
			std::generate(queue_family_indices_.begin(), queue_family_indices_.end(),
				[it = std::move(queue_family_indices.cbegin())]()mutable {return *it++; });

			info.pQueueFamilyIndices = queue_family_indices_.data();
			info.queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices_.size());
		}
	};

	template<>
	struct info<SwapchainKHR>
	{
		using handle_type = SwapchainKHR;
		using type = info_proxy<SwapchainCreateInfoKHR>;
	};

	template<>
	struct info_proxy<SubpassDescription>
	{
		using info_type = SubpassDescription;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pInputAttachments)>> input_attachments;
		vector<decay_to_origin_t<decltype(info.pColorAttachments)>> color_attachments;
		vector<decay_to_origin_t<decltype(info.pResolveAttachments)>> resolve_attachments;
		optional<decay_to_origin_t<decltype(info.pDepthStencilAttachment)>> depth_attachment;
		vector<decay_to_origin_t<decltype(info.pPreserveAttachments)>> preserve_attachments;

		template<
			typename T = decltype(input_attachments),
			typename U = decltype(color_attachments),
			typename V = decltype(resolve_attachments),
			typename W = decltype(depth_attachment),
			typename X = decltype(preserve_attachments),
			typename Y = decltype(info),
			typename = enable_if_convertible_t<T, decltype(input_attachments)>,
			typename = enable_if_convertible_t<U, decltype(color_attachments)>,
			typename = enable_if_convertible_t<V, decltype(resolve_attachments)>,
			typename = enable_if_convertible_t<W, decltype(depth_attachment)>,
			typename = enable_if_convertible_t<X, decltype(preserve_attachments)>,
			typename = enable_if_convertible_t<Y, decltype(info)>
		>
			explicit info_proxy(
				T&& i_a = T{},
				U&& c_a = U{},
				V&& r_a = V{},
				W&& d_a = W{},
				X&& p_a = X{},
				Y&& info = Y{}
			) :
			input_attachments(std::forward<T>(i_a)),
			color_attachments(std::forward<U>(c_a)),
			resolve_attachments(std::forward<V>(r_a)),
			depth_attachment(std::forward<W>(d_a)),
			preserve_attachments(std::forward<X>(p_a)),
			info(std::forward<Y>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			input_attachments(info.pInputAttachments, info.pInputAttachments + info.inputAttachmentCount),
			color_attachments(info.pColorAttachments, info.pColorAttachments + info.colorAttachmentCount),
			resolve_attachments(info.pResolveAttachments, info.pResolveAttachments + info.colorAttachmentCount),
			depth_attachment(info.pDepthStencilAttachment),
			preserve_attachments(info.pPreserveAttachments, info.pPreserveAttachments + info.preserveAttachmentCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			input_attachments(right.input_attachments),
			color_attachments(right.color_attachments),
			resolve_attachments(right.resolve_attachments),
			depth_attachment(right.depth_attachment),
			preserve_attachments(right.preserve_attachments),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			input_attachments(std::move(right.input_attachments)),
			color_attachments(std::move(right.color_attachments)),
			resolve_attachments(std::move(right.resolve_attachments)),
			depth_attachment(std::move(right.depth_attachment)),
			preserve_attachments(std::move(right.preserve_attachments)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
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

		info_proxy& operator=(info_proxy&& right) noexcept
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

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
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
	};

	template<>
	struct info_proxy<RenderPassCreateInfo>
	{
		using info_type = RenderPassCreateInfo;

		info_type info;

	private:
		vector<decay_to_origin_t<decltype(info.pSubpasses)>> subpass_descriptions_;

	public:
		vector<decay_to_origin_t<decltype(info.pAttachments)>> attachment_descriptions;
		vector<info_proxy<decltype(subpass_descriptions_)::value_type>> subpass_descriptions;
		vector<decay_to_origin_t<decltype(info.pDependencies)>> dependencies;

		template<
			typename T = decltype(attachment_descriptions),
			typename U = decltype(subpass_descriptions),
			typename V = decltype(dependencies),
			typename W = decltype(info),
			typename = enable_if_convertible_t<T, decltype(attachment_descriptions)>,
			typename = enable_if_convertible_t<U, decltype(subpass_descriptions)>,
			typename = enable_if_convertible_t<V, decltype(dependencies)>,
			typename = enable_if_convertible_t<W, decltype(info)>
		>
			explicit info_proxy(
				T&& a_descriptions = T{},
				U&& s_descriptions = U{},
				V&& dps = V{},
				W&& info = W{}
			) :
			attachment_descriptions(std::forward<T>(a_descriptions)),
			subpass_descriptions(std::forward<U>(s_descriptions)),
			dependencies(std::forward<V>(dps)),
			info(std::forward<W>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			attachment_descriptions(info.pAttachments, info.pAttachments + info.attachmentCount),
			subpass_descriptions(info.pSubpasses, info.pSubpasses + info.subpassCount),
			dependencies(info.pDependencies, info.pDependencies + info.dependencyCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			attachment_descriptions(right.attachment_descriptions),
			subpass_descriptions(right.subpass_descriptions),
			dependencies(right.dependencies),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			attachment_descriptions(std::move(right.attachment_descriptions)),
			subpass_descriptions(std::move(right.subpass_descriptions)),
			dependencies{std::move(right.dependencies)},
			info{std::move(right.info)}
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			attachment_descriptions = right.attachment_descriptions;
			subpass_descriptions = right.subpass_descriptions;
			dependencies = right.dependencies;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			attachment_descriptions = std::move(right.attachment_descriptions);
			subpass_descriptions = std::move(right.subpass_descriptions);
			dependencies = std::move(right.dependencies);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pAttachments = attachment_descriptions.data();
			info.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size());

			subpass_descriptions_.resize(subpass_descriptions.size());
			std::generate(subpass_descriptions_.begin(),
				subpass_descriptions_.end(),
				[&, i = size_t(0)]()mutable {return subpass_descriptions[i++].info; });

			info.pSubpasses = subpass_descriptions_.data();
			info.subpassCount = static_cast<uint32_t>(subpass_descriptions_.size());

			info.pDependencies = dependencies.data();
			info.dependencyCount = static_cast<uint32_t>(dependencies.size());
		}
	};

	template<>
	struct info<RenderPass>
	{
		using handle_type = RenderPass;
		using type = info_proxy<RenderPassCreateInfo>;
	};

	template<>
	struct info_proxy<FramebufferCreateInfo>
	{
		using info_type = FramebufferCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pAttachments)>> image_views;

		template<
			typename T = decltype(image_views),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(image_views)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& p = T{}, U&& i = U{}) :
			image_views(std::forward<T>(p)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			image_views(info.pAttachments, info.pAttachments + info.attachmentCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			image_views(right.image_views),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			image_views(std::move(right.image_views)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			image_views = right.image_views;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			image_views = std::move(right.image_views);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pAttachments = image_views.data();
			info.attachmentCount = static_cast<uint32_t>(image_views.size());
		}
	};

	template<>
	struct info<Framebuffer>
	{
		using handle_type = Framebuffer;
		using type = info_proxy<FramebufferCreateInfo>;
	};

	template<>
	struct info_proxy<ShaderModuleCreateInfo>
	{
		using info_type = ShaderModuleCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pCode)>> codes;

		template<
			typename T = decltype(codes),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(codes)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& c_p = T{}, U&& info = U{}) :
			codes(std::forward<T>(c_p)),
			info(std::forward<U>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			codes(info.pCode, info.pCode + info.codeSize / sizeof(decltype(codes)::value_type)),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			codes(right.codes),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			codes(std::move(right.codes)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			codes = right.codes;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			codes = std::move(right.codes);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pCode = codes.data();
			info.codeSize = codes.size() * sizeof(decltype(codes)::value_type);
		}
	};

	template<>
	struct info<ShaderModule>
	{
		using handle_type = ShaderModule;
		using type = info_proxy<ShaderModuleCreateInfo>;
	};

	template<>
	struct info_proxy<SpecializationInfo>
	{
		using info_type = SpecializationInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pMapEntries)>> map_entries;

		template<
			typename T = decltype(map_entries),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(map_entries)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& p = T{}, U&& i = U{}) :
			map_entries(std::forward<T>(p)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			map_entries(info.pMapEntries, info.pMapEntries + info.mapEntryCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			map_entries(right.map_entries),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			map_entries(std::move(right.map_entries)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			map_entries = right.map_entries;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			map_entries = std::move(right.map_entries);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pMapEntries = map_entries.data();
			info.mapEntryCount = static_cast<uint32_t>(map_entries.size());
		}
	};

	template<>
	struct info_proxy<PipelineShaderStageCreateInfo>
	{
		using info_type = PipelineShaderStageCreateInfo;

		info_type info;
		string entry_name;
		optional<info_proxy<decay_to_origin_t<decltype(info.pSpecializationInfo)>>> specialization_info;

		template<
			typename T = decltype(entry_name),
			typename U = decltype(specialization_info),
			typename V = decltype(info),
			typename = enable_if_convertible_t<T, decltype(entry_name)>,
			typename = enable_if_convertible_t<U, decltype(specialization_info)>,
			typename = enable_if_convertible_t<V, decltype(info)>
		>
			explicit info_proxy(T&& n = T{}, U&& p = U{}, V&& i = V{}) :
			entry_name(std::forward<T>(n)),
			specialization_info(std::forward<U>(p)),
			info(std::forward<V>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			entry_name(info.pName),
			specialization_info(std::forward<std::remove_pointer_t<decltype(
				info.pSpecializationInfo
				)>>(*info.pSpecializationInfo)),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			entry_name(right.entry_name),
			specialization_info(right.specialization_info),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			entry_name(std::move(right.entry_name)),
			specialization_info(std::move(right.specialization_info)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			entry_name = right.entry_name;
			specialization_info = right.specialization_info;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			entry_name = std::move(right.entry_name);
			specialization_info = std::move(right.specialization_info);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pName = entry_name.c_str();
			info.pSpecializationInfo = specialization_info ? &specialization_info->info : nullptr;
		}
	};

	template<>
	struct info_proxy<PipelineVertexInputStateCreateInfo>
	{
		using info_type = PipelineVertexInputStateCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pVertexBindingDescriptions)>> vertex_input_binding_descriptions;
		vector<decay_to_origin_t<decltype(info.pVertexAttributeDescriptions)>> vertex_input_attribute_descriptions;

		template<
			typename T = decltype(vertex_input_binding_descriptions),
			typename U = decltype(vertex_input_attribute_descriptions),
			typename V = decltype(info),
			typename = enable_if_convertible_t<T, decltype(vertex_input_binding_descriptions)>,
			typename = enable_if_convertible_t<U, decltype(vertex_input_attribute_descriptions)>,
			typename = enable_if_convertible_t<V, decltype(info)>
		>
			explicit info_proxy(T&& binding = T{}, U&& attribute = U{}, V&& i = V{}) :
			vertex_input_binding_descriptions(std::forward<T>(binding)),
			vertex_input_attribute_descriptions(std::forward<U>(attribute)),
			info(std::forward<V>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			vertex_input_binding_descriptions(
				info.pVertexAttributeDescriptions,
				info.pVertexAttributeDescriptions + info.vertexAttributeDescriptionCount
			),
			vertex_input_attribute_descriptions(
				info.pVertexBindingDescriptions,
				info.pVertexBindingDescriptions + info.vertexBindingDescriptionCount
			),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			vertex_input_binding_descriptions(right.vertex_input_binding_descriptions),
			vertex_input_attribute_descriptions(right.vertex_input_attribute_descriptions),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			vertex_input_binding_descriptions(std::move(right.vertex_input_binding_descriptions)),
			vertex_input_attribute_descriptions(std::move(right.vertex_input_attribute_descriptions)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			vertex_input_attribute_descriptions = right.vertex_input_attribute_descriptions;
			vertex_input_binding_descriptions = right.vertex_input_binding_descriptions;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			vertex_input_attribute_descriptions = std::move(right.vertex_input_attribute_descriptions);
			vertex_input_binding_descriptions = std::move(right.vertex_input_binding_descriptions);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pVertexAttributeDescriptions = vertex_input_attribute_descriptions.data();
			info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_input_attribute_descriptions.size());

			info.pVertexBindingDescriptions = vertex_input_binding_descriptions.data();
			info.vertexBindingDescriptionCount = static_cast<uint32_t>(vertex_input_binding_descriptions.size());
		}
	};

	template<>
	struct info_proxy<PipelineViewportStateCreateInfo>
	{
		using info_type = PipelineViewportStateCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pViewports)>> viewports;
		vector<decay_to_origin_t<decltype(info.pScissors)>> scissors;

		template<
			typename T = decltype(viewports),
			typename U = decltype(scissors),
			typename V = decltype(info),
			typename = enable_if_convertible_t<T, decltype(viewports)>,
			typename = enable_if_convertible_t<U, decltype(scissors)>,
			typename = enable_if_convertible_t<V, decltype(info)>
		>
			explicit info_proxy(T&& v_ps = T{}, U&& ss = U{}, V&& i = V{}) :
			viewports(std::forward<T>(v_ps)),
			scissors(std::forward<U>(ss)),
			info(std::forward<V>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			viewports(info.pViewports, info.pViewports + info.viewportCount),
			scissors(info.pScissors, info.pScissors + info.scissorCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			viewports(right.viewports),
			scissors(right.scissors),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			viewports(std::move(right.viewports)),
			scissors(std::move(right.scissors)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			viewports = right.viewports;
			scissors = right.scissors;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			viewports = std::move(right.viewports);
			scissors = std::move(right.scissors);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pViewports = viewports.data();
			info.viewportCount = static_cast<uint32_t>(viewports.size());

			info.pScissors = scissors.data();
			info.scissorCount = static_cast<uint32_t>(scissors.size());
		}
	};

	template<>
	struct info_proxy<PipelineColorBlendStateCreateInfo>
	{
		using info_type = PipelineColorBlendStateCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pAttachments)>> attachment_states;

		template<
			typename T = decltype(attachment_states),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(attachment_states)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& a_ss = T{}, U&& i = U{}) :
			attachment_states(std::forward<T>(a_ss)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			attachment_states(info.pAttachments, info.pAttachments + info.attachmentCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			attachment_states(right.attachment_states),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			attachment_states(std::move(right.attachment_states)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			attachment_states = right.attachment_states;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			attachment_states = std::move(right.attachment_states);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pAttachments = attachment_states.data();
			info.attachmentCount = static_cast<uint32_t>(attachment_states.size());
		}
	};

	template<>
	struct info_proxy<PipelineLayoutCreateInfo>
	{
		using info_type = PipelineLayoutCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pSetLayouts)>> set_layouts;
		vector<decay_to_origin_t<decltype(info.pPushConstantRanges)>> push_const_ranges;

		template<
			typename T = decltype(set_layouts),
			typename U = decltype(push_const_ranges),
			typename V = decltype(info),
			typename = enable_if_convertible_t<T, decltype(set_layouts)>,
			typename = enable_if_convertible_t<U, decltype(push_const_ranges)>,
			typename = enable_if_convertible_t<V, decltype(info)>
		>
			explicit info_proxy(T&& layouts = T{}, U&& ranges = U{}, V&& i = V{}) :
			set_layouts(std::forward<T>(layouts)),
			push_const_ranges(std::forward<U>(ranges)),
			info(std::forward<V>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			set_layouts(info.setLayoutCount, info.setLayoutCount + info.pSetLayouts),
			push_const_ranges(info.pPushConstantRanges, info.pPushConstantRanges + info.pushConstantRangeCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			set_layouts(right.set_layouts),
			push_const_ranges(right.push_const_ranges),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			set_layouts(std::move(right.set_layouts)),
			push_const_ranges(std::move(right.push_const_ranges)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			set_layouts = right.set_layouts;
			push_const_ranges = right.push_const_ranges;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			set_layouts = std::move(right.set_layouts);
			push_const_ranges = std::move(right.push_const_ranges);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.setLayoutCount = static_cast<uint32_t>(set_layouts.size());
			info.pSetLayouts = set_layouts.data();

			info.pPushConstantRanges = push_const_ranges.data();
			info.pushConstantRangeCount = static_cast<uint32_t>(push_const_ranges.size());
		}
	};

	template<>
	struct info<PipelineLayout>
	{
		using handle_type = PipelineLayout;
		using type = info_proxy<PipelineLayoutCreateInfo>;
	};

	template<>
	struct info_proxy<GraphicsPipelineCreateInfo>
	{
		using info_type = GraphicsPipelineCreateInfo;

		info_type info;
	private:
		vector<decay_to_origin_t<decltype(info.pStages)>> stages_;

	public:
		vector<info_proxy<decltype(stages_)::value_type>> stages_create_infos;
		optional<info_proxy<decay_to_origin_t<decltype(info.pVertexInputState)>>> vertex_input_state;
		optional<decay_to_origin_t<decltype(info.pInputAssemblyState)>> input_assembly_state;
		optional<decay_to_origin_t<decltype(info.pTessellationState)>> tessellation_state;
		optional<info_proxy<decay_to_origin_t<decltype(info.pViewportState)>>> viewport_state;
		optional<decay_to_origin_t<decltype(info.pRasterizationState)>> rasterization_state_create_info;
		optional<decay_to_origin_t<decltype(info.pMultisampleState)>> multi_sample_state_create_info;
		optional<decay_to_origin_t<decltype(info.pDepthStencilState)>> depth_stencil_state_create_info;
		optional<info_proxy<decay_to_origin_t<decltype(info.pColorBlendState)>>> color_blend_state_create_info;
		optional<decay_to_origin_t<decltype(info.pDynamicState)>> dynamic_state_create_info;

		template<
			typename P = decltype(stages_create_infos),
			typename Q = decltype(vertex_input_state),
			typename R = decltype(input_assembly_state),
			typename S = decltype(tessellation_state),
			typename T = decltype(viewport_state),
			typename U = decltype(rasterization_state_create_info),
			typename V = decltype(multi_sample_state_create_info),
			typename W = decltype(depth_stencil_state_create_info),
			typename X = decltype(color_blend_state_create_info),
			typename Y = decltype(dynamic_state_create_info),
			typename Z = decltype(info),
			typename = enable_if_convertible_t<P, decltype(stages_create_infos)>,
			typename = enable_if_convertible_t<Q, decltype(vertex_input_state)>,
			typename = enable_if_convertible_t<R, decltype(input_assembly_state)>,
			typename = enable_if_convertible_t<S, decltype(tessellation_state)>,
			typename = enable_if_convertible_t<T, decltype(viewport_state)>,
			typename = enable_if_convertible_t<U, decltype(rasterization_state_create_info)>,
			typename = enable_if_convertible_t<V, decltype(multi_sample_state_create_info)>,
			typename = enable_if_convertible_t<W, decltype(depth_stencil_state_create_info)>,
			typename = enable_if_convertible_t<X, decltype(color_blend_state_create_info)>,
			typename = enable_if_convertible_t<Y, decltype(dynamic_state_create_info)>,
			typename = enable_if_convertible_t<Z, decltype(info)>
		>
			explicit info_proxy(
				P&& stages = P{},
				Q&& vertex = Q{},
				R&& input = R{},
				S&& tessellation = S{},
				T&& viewport = T{},
				U&& rasterization = U{},
				V&& multi_sample = V{},
				W&& depth_stencil = W{},
				X&& color_blend = X{},
				Y&& dynamic_state = Y{},
				Z&& info = Z{}
			) :
			stages_create_infos(std::forward<P>(stages)),
			vertex_input_state(std::forward<Q>(vertex)),
			input_assembly_state(std::forward<R>(input)),
			tessellation_state(std::forward<S>(tessellation)),
			viewport_state(std::forward<T>(viewport)),
			rasterization_state_create_info(std::forward<U>(rasterization)),
			multi_sample_state_create_info(std::forward<V>(multi_sample)),
			depth_stencil_state_create_info(std::forward<W>(depth_stencil)),
			color_blend_state_create_info(std::forward<X>(color_blend)),
			dynamic_state_create_info(std::forward<Y>(dynamic_state)),
			info(std::forward<Z>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			stages_create_infos(info.pStages, info.pStages + info.stageCount),
			vertex_input_state(std::forward<std::remove_pointer_t<decltype(
				info.pVertexInputState
				)>>(*info.pVertexInputState)),
			input_assembly_state(std::forward<std::remove_pointer_t<decltype(
				info.pInputAssemblyState
				)>>(*info.pInputAssemblyState)),
			tessellation_state(std::forward<std::remove_pointer_t<decltype(
				info.pTessellationState
				)>>(*info.pTessellationState)),
			viewport_state(std::forward<std::remove_pointer_t<decltype(
				info.pViewportState
				)>>(*info.pViewportState)),
			rasterization_state_create_info(std::forward<std::remove_pointer_t<decltype(
				info.pRasterizationState
				)>>(*info.pRasterizationState)),
			multi_sample_state_create_info(std::forward<std::remove_pointer_t<decltype(
				info.pMultisampleState
				)>>(*info.pMultisampleState)),
			depth_stencil_state_create_info(std::forward<std::remove_pointer_t<decltype(
				info.pDepthStencilState
				)>>(*info.pDepthStencilState)),
			color_blend_state_create_info(std::forward<std::remove_pointer_t<decltype(
				info.pColorBlendState
				)>>(*info.pColorBlendState)),
			dynamic_state_create_info(std::forward<std::remove_pointer_t<decltype(
				info.pDynamicState
				)>>(*info.pDynamicState)),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			stages_create_infos(right.stages_create_infos),
			vertex_input_state(right.vertex_input_state),
			input_assembly_state(right.input_assembly_state),
			tessellation_state(right.tessellation_state),
			viewport_state(right.viewport_state),
			rasterization_state_create_info(right.rasterization_state_create_info),
			multi_sample_state_create_info(right.multi_sample_state_create_info),
			depth_stencil_state_create_info(right.depth_stencil_state_create_info),
			color_blend_state_create_info(right.color_blend_state_create_info),
			dynamic_state_create_info(right.dynamic_state_create_info),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			stages_create_infos(std::move(right.stages_create_infos)),
			vertex_input_state(std::move(right.vertex_input_state)),
			input_assembly_state(std::move(right.input_assembly_state)),
			tessellation_state(std::move(right.tessellation_state)),
			viewport_state(std::move(right.viewport_state)),
			rasterization_state_create_info(std::move(right.rasterization_state_create_info)),
			multi_sample_state_create_info(std::move(right.multi_sample_state_create_info)),
			depth_stencil_state_create_info(std::move(right.depth_stencil_state_create_info)),
			color_blend_state_create_info(std::move(right.color_blend_state_create_info)),
			dynamic_state_create_info(std::move(right.dynamic_state_create_info)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
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

		info_proxy& operator=(info_proxy&& right) noexcept
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

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			stages_.resize(stages_create_infos.size());
			std::generate(
				stages_.begin(),
				stages_.end(),
				[&, it = std::move(stages_create_infos.cbegin())]()mutable  {return it++->info; }
			);

			info.pStages = stages_.data();
			info.stageCount = static_cast<uint32_t>(stages_.size());

			info.pVertexInputState = vertex_input_state ? &vertex_input_state->info : nullptr;

			info.pInputAssemblyState = input_assembly_state ? &*input_assembly_state : nullptr;

			info.pTessellationState = tessellation_state ? &*tessellation_state : nullptr;

			info.pViewportState = viewport_state ? &viewport_state->info : nullptr;

			info.pRasterizationState = rasterization_state_create_info ?
				&*rasterization_state_create_info :
				nullptr;

			info.pMultisampleState = multi_sample_state_create_info ?
				&*multi_sample_state_create_info :
				nullptr;

			info.pDepthStencilState = depth_stencil_state_create_info ?
				&*depth_stencil_state_create_info :
				nullptr;

			info.pColorBlendState = color_blend_state_create_info ?
				&color_blend_state_create_info->info :
				nullptr;

			info.pDynamicState = dynamic_state_create_info ?
				&*dynamic_state_create_info :
				nullptr;
		}
	};

	template<>
	struct info<GraphicsPipeline>
	{
		using handle_type = GraphicsPipeline;
		using type = info_proxy<GraphicsPipelineCreateInfo>;
	};

	template<>
	struct info_proxy<CommandBufferBeginInfo>
	{
		using info_type = CommandBufferBeginInfo;

		info_type info;
		optional<decay_to_origin_t<decltype(info.pInheritanceInfo)>> inheritance_info;

		template<
			typename T = decltype(inheritance_info),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(inheritance_info)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& i_i = T{}, U&& i = U{}) :
			inheritance_info(std::forward<T>(i_i)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			inheritance_info(std::forward<std::remove_pointer_t<decltype(
				info.pInheritanceInfo
				)>>(*info.pInheritanceInfo)),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			inheritance_info(right.inheritance_info),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			inheritance_info(std::move(right.inheritance_info)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			inheritance_info = right.inheritance_info;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			inheritance_info = std::move(right.inheritance_info);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pInheritanceInfo = inheritance_info ? &*inheritance_info : nullptr;
		}
	};

	template<>
	struct info_proxy<RenderPassBeginInfo>
	{
		using info_type = RenderPassBeginInfo;

		info_type info;

		vector<decay_to_origin_t<decltype(info.pClearValues)>> clear_values;

		template<
			typename T = decltype(clear_values),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(clear_values)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(T&& c_vs = T{}, U&& i = U{}) :
			clear_values(std::forward<T>(c_vs)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			clear_values(info.pClearValues, info.pClearValues + info.clearValueCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			clear_values(right.clear_values),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			clear_values(std::move(right.clear_values)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			clear_values = right.clear_values;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			clear_values = std::move(right.clear_values);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pClearValues = clear_values.data();
			info.clearValueCount = static_cast<uint32_t>(clear_values.size());
		}
	};

	template<>
	struct info_proxy<DescriptorSetAllocateInfo>
	{
		using info_type = DescriptorSetAllocateInfo;

		info_type info;

		vector<decay_to_origin_t<decltype(info.pSetLayouts)>> descriptor_set_layouts;

		template<
			typename T = decltype(descriptor_set_layouts),
			typename U = decltype(info),
			typename = enable_if_convertible_t<T, decltype(descriptor_set_layouts)>,
			typename = enable_if_convertible_t<U, decltype(info)>
		>
			explicit info_proxy(
				T&& layouts = T{},
				U&& i = U{}
			) :
			descriptor_set_layouts(std::forward<T>(layouts)),
			info(std::forward<U>(i))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			descriptor_set_layouts(info.pSetLayouts, info.pSetLayouts + info.descriptorSetCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			descriptor_set_layouts(right.descriptor_set_layouts),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			descriptor_set_layouts(std::move(right.descriptor_set_layouts)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			descriptor_set_layouts = right.descriptor_set_layouts;
			info = right.info;
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			descriptor_set_layouts = std::move(right.descriptor_set_layouts);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pSetLayouts = descriptor_set_layouts.data();
			info.descriptorSetCount = static_cast<uint32_t>(descriptor_set_layouts.size());
		}
	};

	template<>
	struct info<DescriptorSet>
	{
		using handle_type = DescriptorSet;
		using type = info_proxy<DescriptorSetAllocateInfo>;
	};

	template<>
	struct info_proxy<SubmitInfo>
	{
		using info_type = SubmitInfo;

		info_type info;

		vector<decay_to_origin_t<decltype(info.pWaitSemaphores)>> descriptor_set_layouts;
		optional<decay_to_origin_t<decltype(info.pWaitDstStageMask)>> dst_stage_mask;
		vector<decay_to_origin_t<decltype(info.pCommandBuffers)>> command_buffers;
		vector<decay_to_origin_t<decltype(info.pSignalSemaphores)>> signal_semaphores;

		template<
			typename T = decltype(descriptor_set_layouts),
			typename U = decltype(dst_stage_mask),
			typename V = decltype(command_buffers),
			typename W = decltype(signal_semaphores),
			typename Y = decltype(info),
			typename = enable_if_convertible_t<T, decltype(descriptor_set_layouts)>,
			typename = enable_if_convertible_t<U, decltype(dst_stage_mask)>,
			typename = enable_if_convertible_t<V, decltype(command_buffers)>,
			typename = enable_if_convertible_t<W, decltype(signal_semaphores)>,
			typename = enable_if_convertible_t<Y, decltype(info)>
		>
			explicit info_proxy(
				T&& w_ss = T{},
				U&& d_s_ms = U{},
				V&& c_bs = V{},
				W&& s_ss = W{},
				Y&& info = Y{}
			) :
			descriptor_set_layouts(std::forward<T>(w_ss)),
			dst_stage_mask(std::forward<U>(d_s_ms)),
			command_buffers(std::forward<V>(c_bs)),
			signal_semaphores(std::forward<W>(s_ss)),
			info(std::forward<Y>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			descriptor_set_layouts(info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount),
			dst_stage_mask(std::forward<std::remove_pointer_t<decltype(
				info.pWaitDstStageMask
				)>>(*info.pWaitDstStageMask)),
			command_buffers(info.pCommandBuffers, info.commandBufferCount),
			signal_semaphores(info.pSignalSemaphores, info.signalSemaphoreCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			descriptor_set_layouts(right.descriptor_set_layouts),
			dst_stage_mask(right.dst_stage_mask),
			command_buffers(right.command_buffers),
			signal_semaphores(right.signal_semaphores),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			descriptor_set_layouts(std::move(right.descriptor_set_layouts)),
			dst_stage_mask(std::move(right.dst_stage_mask)),
			command_buffers(std::move(right.command_buffers)),
			signal_semaphores(std::move(right.signal_semaphores)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			descriptor_set_layouts = right.descriptor_set_layouts;
			dst_stage_mask = right.dst_stage_mask;
			command_buffers = right.command_buffers;
			signal_semaphores = right.signal_semaphores;
			info = right.info;
			set_proxy();
			return *this;
		}

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		info_proxy& operator=(info_proxy&& right) noexcept
		{
			descriptor_set_layouts = std::move(right.descriptor_set_layouts);
			dst_stage_mask = std::move(right.dst_stage_mask);
			command_buffers = std::move(right.command_buffers);
			signal_semaphores = std::move(right.signal_semaphores);
			info = std::move(right.info);
			set_proxy();
			return *this;
		}

		void set_proxy()
		{
			info.pWaitSemaphores = descriptor_set_layouts.data();
			info.waitSemaphoreCount = static_cast<uint32_t>(descriptor_set_layouts.size());

			info.pWaitDstStageMask = dst_stage_mask ? &*dst_stage_mask : nullptr;

			info.pCommandBuffers = command_buffers.data();
			info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

			info.pSignalSemaphores = signal_semaphores.data();
			info.signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores.size());
		}
	};

	template<>
	struct info_proxy<PresentInfoKHR>
	{
		using info_type = PresentInfoKHR;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pWaitSemaphores)>> semaphores;
		vector<decay_to_origin_t<decltype(info.pSwapchains)>> swap_chains;
		vector<decay_to_origin_t<decltype(info.pImageIndices)>> image_indices;
		vector<decay_to_origin_t<decltype(info.pResults)>> results;

		template<
			typename T = decltype(semaphores),
			typename U = decltype(swap_chains),
			typename V = decltype(image_indices),
			typename W = decltype(results),
			typename X = decltype(info),
			typename = enable_if_convertible_t<T, decltype(semaphores)>,
			typename = enable_if_convertible_t<U, decltype(swap_chains)>,
			typename = enable_if_convertible_t<V, decltype(image_indices)>,
			typename = enable_if_convertible_t<W, decltype(results)>,
			typename = enable_if_convertible_t<X, decltype(info)>
		>
			explicit info_proxy(
				T&& w_ss = T{},
				U&& s_cs = U{},
				V&& indices = V{},
				W&& r_ss = W{},
				X&& info = X{}
			) :
			semaphores(std::forward<T>(w_ss)),
			swap_chains(std::forward<U>(s_cs)),
			image_indices(std::forward<V>(indices)),
			results(std::forward<W>(r_ss)),
			info(std::forward<X>(info))
		{
			set_proxy();
		}

		template<typename InfoType = info_type, typename = enable_if_decay_same_t<InfoType, info_type>>
		info_proxy(InfoType&& info) :
			semaphores(info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount),
			swap_chains(info.pSwapchains, info.pSwapchains + info.swapchainCount),
			image_indices(info.pImageIndices, info.pImageIndices + info.swapchainCount),
			results(info.pResults, info.pResults + info.swapchainCount),
			info(std::forward<InfoType>(info))
		{
			set_proxy();
		}

		info_proxy(const info_proxy& right) :
			semaphores(right.semaphores),
			swap_chains(right.swap_chains),
			image_indices(right.image_indices),
			results(right.results),
			info(right.info)
		{
			set_proxy();
		}

		info_proxy(info_proxy&& right) noexcept :
			semaphores(std::move(right.semaphores)),
			swap_chains(std::move(right.swap_chains)),
			image_indices(std::move(right.image_indices)),
			results(std::move(right.results)),
			info(std::move(right.info))
		{
			set_proxy();
		}

		info_proxy& operator=(const info_proxy& right)
		{
			semaphores = right.semaphores;
			swap_chains = right.swap_chains;
			image_indices = right.image_indices;
			results = right.results;
			info = right.info;
			set_proxy();
			return *this;
		}

		info_proxy& operator=(info_proxy&& right) noexcept
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

		operator info_type& () { return info; }
		operator const info_type& ()const { return info; }

		void set_proxy()
		{
			info.pWaitSemaphores = semaphores.data();
			info.waitSemaphoreCount = static_cast<uint32_t>(semaphores.size());

			info.pSwapchains = swap_chains.data();
			info.swapchainCount = static_cast<uint32_t>(swap_chains.size());

			info.pImageIndices = image_indices.data();

			info.pResults = results.data();
		}
	};

	template<typename T>
	using info_t = typename info<T>::type;
}
