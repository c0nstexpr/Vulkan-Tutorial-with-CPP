#pragma once

#include "vulkan/utility/utility_core.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vulkan::utility
{
	template<typename T>
	struct info;

	template<>
	struct info<DebugUtilsMessengerEXT>
	{
		using handle_type = DebugUtilsMessengerEXT;
		using base_info_type = DebugUtilsMessengerCreateInfoEXT;
		using type = base_info_type;
	};

	struct surface_khr_create_info
	{
		using glfw_window_pointer = GLFWwindow*;

		glfw_window_pointer window;
	};

	template<>
	struct info<SurfaceKHR>
	{
		using handle_type = SurfaceKHR;
		using base_info_type = surface_khr_create_info;
		using type = base_info_type;
	};

	template<>
	struct info<DeviceMemory>
	{
		using handle_type = DeviceMemory;
		using base_info_type = MemoryAllocateInfo;
		using type = base_info_type;
	};

	template<>
	struct info<ImageView>
	{
		using handle_type = ImageView;
		using base_info_type = ImageViewCreateInfo;
		using type = base_info_type;
	};

	template<>
	struct info<Sampler>
	{
		using handle_type = Sampler;
		using base_info_type = SamplerCreateInfo;
		using type = base_info_type;
	};

	template<>
	struct info<CommandPool>
	{
		using handle_type = CommandPool;
		using base_info_type = CommandPoolCreateInfo;
		using type = base_info_type;
	};

	template<>
	struct info<CommandBuffer>
	{
		using handle_type = CommandBuffer;
		using base_info_type = CommandBufferAllocateInfo;
		using type = base_info_type;
	};

	template<>
	struct info<Semaphore>
	{
		using handle_type = Semaphore;
		using base_info_type = SemaphoreCreateInfo;
		using type = base_info_type;
	};

	template<>
	struct info<Fence>
	{
		using handle_type = Fence;
		using base_info_type = FenceCreateInfo;
		using type = base_info_type;
	};

	template<typename T>
	struct info_proxy;

	template<typename T>
	struct info_proxy_base
	{
		using base_info_type = T;

		T info;

		constexpr info_proxy_base(base_info_type i = base_info_type{}) noexcept :
			info(std::move(i)) {}

#define DEFAULT_COPY_CONSTRUCTOR(TYPE) TYPE(const TYPE& right) : base(right.info) { property_copy(right); }
#define DEFAULT_MOVE_CONSTRUCTOR(TYPE)\
		TYPE(TYPE&& right) noexcept : base(std::move(right.info)) { property_move(std::move(right)); }

#define DEFAULT_COPY_ASSIGN_CONSTRUCTOR(TYPE)\
		TYPE& operator=(const TYPE& right) { info = right.info; property_copy(right);return *this; }

#define DEFAULT_MOVE_ASSIGN_CONSTRUCTOR(TYPE)\
		TYPE& operator=(TYPE&& right) noexcept\
		{\
			info = std::move(right.info);\
			property_move(std::move(right));\
			return *this;\
		}

#define DEFAULT_DECONSTRUCTOR(TYPE) ~TYPE() = default;

#define DEFAULT_RULE_OF_5(TYPE) DEFAULT_COPY_CONSTRUCTOR(TYPE)\
		DEFAULT_MOVE_CONSTRUCTOR(TYPE)\
		DEFAULT_COPY_ASSIGN_CONSTRUCTOR(TYPE)\
		DEFAULT_MOVE_ASSIGN_CONSTRUCTOR(TYPE)\
		DEFAULT_DECONSTRUCTOR(TYPE)

		constexpr operator base_info_type&() { return info; }

		constexpr operator const base_info_type&() const { return info; }
	};

	template<>
	struct info_proxy<ApplicationInfo> : info_proxy_base<ApplicationInfo>
	{
		using base = info_proxy_base;

	private:
		string application_name_;
		string engine_name_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(application_name_),
			decltype(engine_name_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(application_name_)& get_application_name() const noexcept;
		void set_application_name(decltype(application_name_) value);

		constexpr const decltype(engine_name_)& get_engine_name() const noexcept;
		void set_engine_name(decltype(engine_name_) value);

		const property<info_proxy, string> application_name_property{
			*this,
			&info_proxy::get_application_name,
			&info_proxy::set_application_name
		};

		const property<info_proxy, string> engine_name_property{
			*this,
			&info_proxy::get_engine_name,
			&info_proxy::set_engine_name
		};
	};

	template<>
	struct info_proxy<InstanceCreateInfo> : info_proxy_base<InstanceCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<const char*> extension_names_;
		vector<const char*> layer_names_;

		optional<info_proxy<decay_to_origin_t<decltype(info.pApplicationInfo)>>> application_info_;
		vector<string> extension_name_strs_;
		vector<string> layer_name_strs_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(application_info_),
			decltype(extension_name_strs_) = {},
			decltype(layer_name_strs_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(application_info_)& get_application_info() const noexcept;
		void set_application_info(decltype(application_info_));

		constexpr const decltype(extension_name_strs_)& get_extension_name_strs() const noexcept;
		void set_extension_name_strs(decltype(extension_name_strs_));

		constexpr const decltype(layer_name_strs_)& get_layer_name_strs() const noexcept;
		void set_layer_name_strs(decltype(layer_name_strs_));

		const property<info_proxy, decltype(application_info_)> application_info_property{
			*this,
			&info_proxy::get_application_info,
			&info_proxy::set_application_info,
		};

		const property<info_proxy, decltype(extension_name_strs_)> extension_name_strs_property{
			*this,
			&info_proxy::get_extension_name_strs,
			&info_proxy::set_extension_name_strs,
		};

		const property<info_proxy, decltype(layer_name_strs_)> layer_name_strs_property{
			*this,
			&info_proxy::get_layer_name_strs,
			&info_proxy::set_layer_name_strs,
		};
	};

	template<>
	struct info<Instance>
	{
		using handle_type = Instance;
		using base_info_type = InstanceCreateInfo;
		using type = info_proxy<InstanceCreateInfo>;
	};

	template<>
	struct info_proxy<DeviceQueueCreateInfo> : info_proxy_base<DeviceQueueCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pQueuePriorities)>> priorities_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(priorities_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(priorities_)& get_priorities() const noexcept;
		void set_priorities(decltype(priorities_));

		const property<info_proxy, decltype(priorities_)> priorities_property{
			*this,
			&info_proxy::get_priorities,
			&info_proxy::set_priorities
		};

		constexpr bool operator>(const info_proxy& right) const noexcept
		{
			return info.queueFamilyIndex > right.info.queueFamilyIndex;
		}

		constexpr bool operator<(const info_proxy& right) const noexcept
		{
			return info.queueFamilyIndex < right.info.queueFamilyIndex;
		}

		constexpr bool operator==(const info_proxy& right) const noexcept
		{
			return info.queueFamilyIndex == right.info.queueFamilyIndex;
		}

		constexpr bool operator>=(const info_proxy& right) const noexcept { return *this > right || *this == right; }

		constexpr bool operator<=(const info_proxy& right) const noexcept { return *this < right || *this == right; }

		constexpr bool operator!=(const info_proxy& right) const noexcept { return !(*this == right); }
	};

	template<>
	struct info_proxy<DeviceCreateInfo> : info_proxy_base<DeviceCreateInfo>
	{
		using base = info_proxy_base;

	private:
		using queue_create_info = decay_to_origin_t<decltype(info.pQueueCreateInfos)>;

		vector<queue_create_info> queue_create_infos_;
		vector<const char*> extension_names_;

		//The "queueFamilyIndex" in "DeviceQueueCreateInfo" is required to be unique
		set<info_proxy<queue_create_info>> queue_create_infos_set_;
		vector<string> extension_name_strs_;
		optional<decay_to_origin_t<decltype(info.pEnabledFeatures)>> features_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(queue_create_infos_set_),
			decltype(extension_name_strs_) = {},
			decltype(features_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(queue_create_infos_set_)& get_queue_create_info_set() const noexcept;
		void set_queue_create_info_set(decltype(queue_create_infos_set_));

		constexpr const decltype(extension_name_strs_)& get_extension_name_strs() const noexcept;
		void set_extension_names(decltype(extension_name_strs_));

		constexpr const decltype(features_)& get_features() const noexcept;
		void set_features(decltype(features_));

		const property<info_proxy, decltype(queue_create_infos_set_)> queue_create_infos_set_property{
			*this,
			&info_proxy::get_queue_create_info_set,
			&info_proxy::set_queue_create_info_set,
		};
		const property<info_proxy, decltype(extension_name_strs_)> extension_name_strs_property{
			*this,
			&info_proxy::get_extension_name_strs,
			&info_proxy::set_extension_names,
		};
		const property<info_proxy, decltype(features_)> features_property{
			*this,
			&info_proxy::get_features,
			&info_proxy::set_features,
		};
	};

	template<>
	struct info<Device>
	{
		using handle_type = Device;
		using base_info_type = DeviceCreateInfo;
		using type = info_proxy<DeviceCreateInfo>;
	};

	template<>
	struct info_proxy<SwapchainCreateInfoKHR> : info_proxy_base<SwapchainCreateInfoKHR>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pQueueFamilyIndices)>> queue_family_indices_;
		set<std::decay_t<decltype(queue_family_indices_)::value_type>> queue_family_indices_set_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(queue_family_indices_set_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(queue_family_indices_set_)& get_queue_family_indices_set() const noexcept;
		void set_queue_family_indices_set(decltype(queue_family_indices_set_));

		const property<info_proxy, decltype(queue_family_indices_set_)>
		queue_family_indices_set_property{
			*this,
			&info_proxy::get_queue_family_indices_set,
			&info_proxy::set_queue_family_indices_set,
		};
	};

	template<>
	struct info<SwapchainKHR>
	{
		using handle_type = SwapchainKHR;
		using base_info_type = SwapchainCreateInfoKHR;
		using type = info_proxy<SwapchainCreateInfoKHR>;
	};

	template<>
	struct info_proxy<SubpassDescription> : info_proxy_base<SubpassDescription>
	{
		using base = info_proxy_base;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	private:
		vector<decay_to_origin_t<decltype(info.pInputAttachments)>> input_attachments_;
		vector<decay_to_origin_t<decltype(info.pColorAttachments)>> color_attachments_;
		vector<decay_to_origin_t<decltype(info.pResolveAttachments)>> resolve_attachments_;
		optional<decay_to_origin_t<decltype(info.pDepthStencilAttachment)>> depth_attachment_;
		vector<decay_to_origin_t<decltype(info.pPreserveAttachments)>> preserve_attachments_;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(input_attachments_),
			decltype(color_attachments_) = {},
			decltype(resolve_attachments_) = {},
			decltype(depth_attachment_) = {},
			decltype(preserve_attachments_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(input_attachments_)& get_input_attachments() const noexcept;
		void set_input_attachments(decltype(input_attachments_));

		constexpr const decltype(color_attachments_)& get_color_attachments() const noexcept;
		void set_color_attachments(decltype(color_attachments_));

		constexpr const decltype(resolve_attachments_)& get_resolve_attachments() const noexcept;
		void set_resolve_attachments(decltype(resolve_attachments_));

		constexpr const decltype(depth_attachment_)& get_depth_attachment() const noexcept;
		void set_depth_attachment(decltype(depth_attachment_));

		constexpr const decltype(preserve_attachments_)& get_preserve_attachments() const noexcept;
		void set_preserve_attachments(decltype(preserve_attachments_));

		const property<info_proxy, decltype(input_attachments_)> input_attachments_property{
			*this,
			&info_proxy::get_input_attachments,
			&info_proxy::set_input_attachments
		};
		const property<info_proxy, decltype(color_attachments_)> color_attachments_property{
			*this,
			&info_proxy::get_color_attachments,
			&info_proxy::set_color_attachments
		};
		const property<info_proxy, decltype(resolve_attachments_)> resolve_attachments_property{
			*this,
			&info_proxy::get_resolve_attachments,
			&info_proxy::set_resolve_attachments
		};
		const property<info_proxy, decltype(depth_attachment_)> depth_attachment_property{
			*this,
			&info_proxy::get_depth_attachment,
			&info_proxy::set_depth_attachment
		};
		const property<info_proxy, decltype(preserve_attachments_)> preserve_attachments_property{
			*this,
			&info_proxy::get_preserve_attachments,
			&info_proxy::set_preserve_attachments
		};
	};

	template<>
	struct info_proxy<RenderPassCreateInfo> : info_proxy_base<RenderPassCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pSubpasses)>> subpass_descriptions_;
		vector<decay_to_origin_t<decltype(info.pAttachments)>> attachment_descriptions_;
		vector<info_proxy<decltype(subpass_descriptions_)::value_type>> subpass_descriptions_info_proxies_;
		vector<decay_to_origin_t<decltype(info.pDependencies)>> dependencies_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(attachment_descriptions_),
			decltype(subpass_descriptions_info_proxies_) = {},
			decltype(dependencies_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(attachment_descriptions_)& get_attachment_descriptions() const noexcept;
		void set_attachment_descriptions(decltype(attachment_descriptions_));

		constexpr const decltype(subpass_descriptions_info_proxies_)&
		get_subpass_descriptions_info_proxies() const noexcept;
		void set_subpass_descriptions_info_proxies(decltype(subpass_descriptions_info_proxies_));

		constexpr const decltype(dependencies_)& get_dependencies() const noexcept;
		void set_dependencies(decltype(dependencies_));

		const property<info_proxy, decltype(attachment_descriptions_)> attachment_descriptions_property{
			*this,
			&info_proxy::get_attachment_descriptions,
			&info_proxy::set_attachment_descriptions
		};
		const property<info_proxy, decltype(subpass_descriptions_info_proxies_)>
		subpass_descriptions_info_proxies_property{
			*this,
			&info_proxy::get_subpass_descriptions_info_proxies,
			&info_proxy::set_subpass_descriptions_info_proxies
		};
		const property<info_proxy, decltype(dependencies_)> dependencies_property{
			*this,
			&info_proxy::get_dependencies,
			&info_proxy::set_dependencies
		};
	};

	template<>
	struct info<RenderPass>
	{
		using handle_type = RenderPass;
		using base_info_type = RenderPassCreateInfo;
		using type = info_proxy<RenderPassCreateInfo>;
	};

	template<>
	struct info_proxy<FramebufferCreateInfo> : info_proxy_base<FramebufferCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pAttachments)>> image_views_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(image_views_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(image_views_)& get_image_views() const noexcept;
		void set_image_views(decltype(image_views_));

		const property<info_proxy, decltype(image_views_)> image_views_property{
			*this,
			&info_proxy::get_image_views,
			&info_proxy::set_image_views
		};
	};

	template<>
	struct info<Framebuffer>
	{
		using handle_type = Framebuffer;
		using base_info_type = FramebufferCreateInfo;
		using type = info_proxy<FramebufferCreateInfo>;
	};

	template<>
	struct info_proxy<ShaderModuleCreateInfo> : info_proxy_base<ShaderModuleCreateInfo>
	{
		using base = info_proxy_base<ShaderModuleCreateInfo>;

	private:
		vector<decay_to_origin_t<decltype(info.pCode)>> codes_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(codes_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(codes_)& get_codes() const noexcept;
		void set_codes(decltype(codes_));

		const property<info_proxy, decltype(codes_)> codes_property{
			*this,
			&info_proxy::get_codes,
			&info_proxy::set_codes,
		};
	};

	template<>
	struct info<ShaderModule>
	{
		using handle_type = ShaderModule;
		using base_info_type = ShaderModuleCreateInfo;
		using type = info_proxy<ShaderModuleCreateInfo>;
	};

	template<>
	struct info_proxy<ImageCreateInfo> : info_proxy_base<ImageCreateInfo>
	{
		using base = info_proxy_base<ImageCreateInfo>;

	private:
		vector<decay_to_origin_t<decltype(info.pQueueFamilyIndices)>> queue_family_indices_;
		set<std::decay_t<decltype(queue_family_indices_)::value_type>> queue_family_indices_set_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(queue_family_indices_set_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(queue_family_indices_set_)& get_queue_family_indices_set() const noexcept;
		void set_queue_family_indices_set(decltype(queue_family_indices_set_));

		const property<info_proxy, decltype(queue_family_indices_set_)>
		queue_family_indices_set_property{
			*this,
			&info_proxy::get_queue_family_indices_set,
			&info_proxy::set_queue_family_indices_set,
		};
	};

	template<>
	struct info<Image>
	{
		using handle_type = Image;
		using base_info_type = ImageCreateInfo;
		using type = info_proxy<ImageCreateInfo>;
	};

	template<>
	struct info_proxy<SpecializationInfo> : info_proxy_base<SpecializationInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pMapEntries)>> map_entries_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(map_entries_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(map_entries_)& get_map_entries() const noexcept;
		void set_map_entries(decltype(map_entries_));

		const property<info_proxy, decltype(map_entries_)> map_entries_property{
			*this,
			&info_proxy::get_map_entries,
			&info_proxy::set_map_entries
		};
	};

	template<>
	struct info_proxy<PipelineShaderStageCreateInfo> : info_proxy_base<PipelineShaderStageCreateInfo>
	{
		using base = info_proxy_base;

	private:
		string entry_name_;
		optional<info_proxy<decay_to_origin_t<decltype(info.pSpecializationInfo)>>> specialization_info_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;
	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(entry_name_),
			decltype(specialization_info_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(entry_name_)& get_entry_name() const noexcept;
		void set_entry_name(decltype(entry_name_));

		constexpr const decltype(specialization_info_)& get_specialization_info() const noexcept;
		void set_specialization_info(decltype(specialization_info_));

		const property<info_proxy, decltype(entry_name_)> entry_name_property{
			*this,
			&info_proxy::get_entry_name,
			&info_proxy::set_entry_name
		};
		const property<info_proxy, decltype(specialization_info_)> specialization_info_property{
			*this,
			&info_proxy::get_specialization_info,
			&info_proxy::set_specialization_info
		};
	};

	template<>
	struct info_proxy<PipelineVertexInputStateCreateInfo> : info_proxy_base<PipelineVertexInputStateCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pVertexBindingDescriptions)>> vertex_input_binding_descriptions_;
		vector<decay_to_origin_t<decltype(info.pVertexAttributeDescriptions)>> vertex_input_attribute_descriptions_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(vertex_input_binding_descriptions_),
			decltype(vertex_input_attribute_descriptions_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(vertex_input_binding_descriptions_)&
		get_vertex_input_binding_descriptions() const noexcept;
		void set_vertex_input_binding_descriptions(decltype(vertex_input_binding_descriptions_));

		constexpr const decltype(vertex_input_attribute_descriptions_)&
		get_vertex_input_attribute_descriptions() const noexcept;
		void set_vertex_input_attribute_descriptions(decltype(vertex_input_attribute_descriptions_));

		const property<info_proxy, decltype(vertex_input_binding_descriptions_)>
		vertex_input_binding_descriptions_property{
			*this,
			&info_proxy::get_vertex_input_binding_descriptions,
			&info_proxy::set_vertex_input_binding_descriptions
		};
		const property<info_proxy, decltype(vertex_input_attribute_descriptions_)>
		vertex_input_attribute_descriptions_property{
			*this,
			&info_proxy::get_vertex_input_attribute_descriptions,
			&info_proxy::set_vertex_input_attribute_descriptions
		};
	};

	template<>
	struct info_proxy<PipelineViewportStateCreateInfo> : info_proxy_base<PipelineViewportStateCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pViewports)>> viewports_;
		vector<decay_to_origin_t<decltype(info.pScissors)>> scissors_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(viewports_),
			decltype(scissors_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(viewports_)& get_viewports() const noexcept;
		void set_viewports(decltype(viewports_));

		constexpr const decltype(scissors_)& get_scissors() const noexcept;
		void set_scissors(decltype(scissors_));

		const property<info_proxy, decltype(viewports_)> viewports_property{
			*this,
			&info_proxy::get_viewports,
			&info_proxy::set_viewports
		};
		const property<info_proxy, decltype(scissors_)> scissors_property{
			*this,
			&info_proxy::get_scissors,
			&info_proxy::set_scissors
		};
	};

	template<>
	struct info_proxy<PipelineColorBlendStateCreateInfo> : info_proxy_base<PipelineColorBlendStateCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pAttachments)>> attachment_states_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(attachment_states_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(attachment_states_)& get_attachment_states() const noexcept;
		void set_attachment_states(decltype(attachment_states_));

		const property<info_proxy, decltype(attachment_states_)> attachment_states_property{
			*this,
			&info_proxy::get_attachment_states,
			&info_proxy::set_attachment_states
		};
	};

	template<>
	struct info_proxy<PipelineLayoutCreateInfo> : info_proxy_base<PipelineLayoutCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pSetLayouts)>> set_layouts_;
		vector<decay_to_origin_t<decltype(info.pPushConstantRanges)>> push_const_ranges_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(set_layouts_),
			decltype(push_const_ranges_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(set_layouts_)& get_set_layouts() const noexcept;
		void set_set_layouts(decltype(set_layouts_));

		constexpr const decltype(push_const_ranges_)& get_push_const_ranges() const noexcept;
		void set_push_const_ranges(decltype(push_const_ranges_));

		const property<info_proxy, decltype(set_layouts_)> set_layouts_property{
			*this,
			&info_proxy::get_set_layouts,
			&info_proxy::set_set_layouts
		};
		const property<info_proxy, decltype(push_const_ranges_)> push_const_ranges_property{
			*this,
			&info_proxy::get_push_const_ranges,
			&info_proxy::set_push_const_ranges
		};
	};

	template<>
	struct info<PipelineLayout>
	{
		using handle_type = PipelineLayout;
		using base_info_type = PipelineLayoutCreateInfo;
		using type = info_proxy<PipelineLayoutCreateInfo>;
	};

	struct graphics_pipeline_create_info : GraphicsPipelineCreateInfo
	{
		using base = GraphicsPipelineCreateInfo;

		PipelineCache cache;
	};

	template<>
	struct info_proxy<graphics_pipeline_create_info> : info_proxy_base<graphics_pipeline_create_info>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pStages)>> stages_;

		vector<info_proxy<decltype(stages_)::value_type>> stage_info_proxies_;
		optional<info_proxy<decay_to_origin_t<decltype(info.pVertexInputState)>>> vertex_input_state_;
		optional<decay_to_origin_t<decltype(info.pInputAssemblyState)>> input_assembly_state_;
		optional<decay_to_origin_t<decltype(info.pTessellationState)>> tessellation_state_;
		optional<info_proxy<decay_to_origin_t<decltype(info.pViewportState)>>> viewport_state_;
		optional<decay_to_origin_t<decltype(info.pRasterizationState)>> rasterization_state_;
		optional<decay_to_origin_t<decltype(info.pMultisampleState)>> multi_sample_state_;
		optional<decay_to_origin_t<decltype(info.pDepthStencilState)>> depth_stencil_state_;
		optional<info_proxy<decay_to_origin_t<decltype(info.pColorBlendState)>>> color_blend_state_;
		optional<decay_to_origin_t<decltype(info.pDynamicState)>> dynamic_state_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;
	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(stage_info_proxies_),
			decltype(vertex_input_state_) = {},
			decltype(input_assembly_state_) = {},
			decltype(tessellation_state_) = {},
			decltype(viewport_state_) = {},
			decltype(rasterization_state_) = {},
			decltype(multi_sample_state_) = {},
			decltype(depth_stencil_state_) = {},
			decltype(color_blend_state_) = {},
			decltype(dynamic_state_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(stage_info_proxies_)& get_stage_info_proxies() const noexcept;
		void set_stage_info_proxies(decltype(stage_info_proxies_));

		constexpr const decltype(vertex_input_state_)& get_vertex_input_state() const noexcept;
		void set_vertex_input_state(decltype(vertex_input_state_));

		constexpr const decltype(input_assembly_state_)& get_input_assembly_state() const noexcept;
		void set_input_assembly_state(decltype(input_assembly_state_));

		constexpr const decltype(tessellation_state_)& get_tessellation_state() const noexcept;
		void set_tessellation_state(decltype(tessellation_state_));

		constexpr const decltype(viewport_state_)& get_viewport_state() const noexcept;
		void set_viewport_state(decltype(viewport_state_));

		constexpr const decltype(rasterization_state_)& get_rasterization_state() const noexcept;
		void set_rasterization_state(decltype(rasterization_state_));

		constexpr const decltype(multi_sample_state_)& get_multi_sample_state() const noexcept;
		void set_multi_sample_state(decltype(multi_sample_state_));

		constexpr const decltype(depth_stencil_state_)& get_depth_stencil_state() const noexcept;
		void set_depth_stencil_state(decltype(depth_stencil_state_));

		constexpr const decltype(color_blend_state_)& get_color_blend_state() const noexcept;
		void set_color_blend_state(decltype(color_blend_state_));

		constexpr const decltype(dynamic_state_)& get_dynamic_state() const noexcept;
		void set_dynamic_state(decltype(dynamic_state_));

		const property<info_proxy, decltype(stage_info_proxies_)> stages_info_proxies_property{
			*this,
			&info_proxy::get_stage_info_proxies,
			&info_proxy::set_stage_info_proxies,
		};
		const property<info_proxy, decltype(vertex_input_state_)> vertex_input_state_property{
			*this,
			&info_proxy::get_vertex_input_state,
			&info_proxy::set_vertex_input_state,
		};
		const property<info_proxy, decltype(input_assembly_state_)> input_assembly_state_property{
			*this,
			&info_proxy::get_input_assembly_state,
			&info_proxy::set_input_assembly_state,
		};
		const property<info_proxy, decltype(tessellation_state_)> tessellation_state_property{
			*this,
			&info_proxy::get_tessellation_state,
			&info_proxy::set_tessellation_state,
		};
		const property<info_proxy, decltype(viewport_state_)> viewport_state_property{
			*this,
			&info_proxy::get_viewport_state,
			&info_proxy::set_viewport_state,
		};
		const property<info_proxy, decltype(rasterization_state_)> rasterization_state_property{
			*this,
			&info_proxy::get_rasterization_state,
			&info_proxy::set_rasterization_state,
		};
		const property<info_proxy, decltype(multi_sample_state_)> multi_sample_state_property{
			*this,
			&info_proxy::get_multi_sample_state,
			&info_proxy::set_multi_sample_state,
		};
		const property<info_proxy, decltype(depth_stencil_state_)> depth_stencil_state_property{
			*this,
			&info_proxy::get_depth_stencil_state,
			&info_proxy::set_depth_stencil_state,
		};
		const property<info_proxy, decltype(color_blend_state_)> color_blend_state_property{
			*this,
			&info_proxy::get_color_blend_state,
			&info_proxy::set_color_blend_state,
		};
		const property<info_proxy, decltype(dynamic_state_)> dynamic_state_property{
			*this,
			&info_proxy::get_dynamic_state,
			&info_proxy::set_dynamic_state,
		};
	};

	template<>
	struct info<GraphicsPipeline>
	{
		using handle_type = GraphicsPipeline;
		using base_info_type = graphics_pipeline_create_info;
		using type = info_proxy<graphics_pipeline_create_info>;
	};

	template<>
	struct info_proxy<CommandBufferBeginInfo> : info_proxy_base<CommandBufferBeginInfo>
	{
		using base = info_proxy_base;

	private:
		optional<decay_to_origin_t<decltype(info.pInheritanceInfo)>> inheritance_info_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;
	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(inheritance_info_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(inheritance_info_)& get_inheritance_info() const noexcept;
		void set_inheritance_info(decltype(inheritance_info_));

		const property<info_proxy, decltype(inheritance_info_)> inheritance_info_property{
			*this,
			&info_proxy::get_inheritance_info,
			&info_proxy::set_inheritance_info
		};
	};

	template<>
	struct info_proxy<RenderPassBeginInfo> : info_proxy_base<RenderPassBeginInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pClearValues)>> clear_values_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(clear_values_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(clear_values_)& get_clear_values() const noexcept;
		void set_clear_values(decltype(clear_values_));

		const property<info_proxy, decltype(clear_values_)> clear_values_property{
			*this,
			&info_proxy::get_clear_values,
			&info_proxy::set_clear_values
		};
	};

	template<>
	struct info_proxy<DescriptorSetLayoutBinding> : info_proxy_base<DescriptorSetLayoutBinding>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pImmutableSamplers)>> samplers_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(samplers_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(samplers_)& get_samplers() const noexcept;
		void set_samplers(decltype(samplers_));

		const property<info_proxy, decltype(samplers_)> samplers_property{
			*this,
			&info_proxy::get_samplers,
			&info_proxy::set_samplers
		};
	};

	template<>
	struct info_proxy<DescriptorSetLayoutCreateInfo> : info_proxy_base<DescriptorSetLayoutCreateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pBindings)>> bindings_;

		vector<info_proxy<decay_to_origin_t<decltype(info.pBindings)>>> bindings_proxies_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(bindings_proxies_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(bindings_proxies_)& get_binding_proxies() const noexcept;
		void set_binding_proxies(decltype(bindings_proxies_));

		const property<info_proxy, decltype(bindings_proxies_)> bindings_proxies_property{
			*this,
			&info_proxy::get_binding_proxies,
			&info_proxy::set_binding_proxies
		};
	};

	template<>
	struct info<DescriptorSetLayout>
	{
		using handle_type = DescriptorSetLayout;
		using base_info_type = DescriptorSetLayoutCreateInfo;
		using type = info_proxy<DescriptorSetLayoutCreateInfo>;
	};

	template<>
	struct info_proxy<DescriptorPoolCreateInfo> : info_proxy_base<DescriptorPoolCreateInfo>
	{
		using base = info_proxy_base;
	private:
		vector<decay_to_origin_t<decltype(info.pPoolSizes)>> pool_sizes_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(pool_sizes_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(pool_sizes_)& get_pool_sizes() const noexcept;
		void set_pool_sizes(decltype(pool_sizes_));

		const property<info_proxy, decltype(pool_sizes_)> pool_sizes_property{
			*this,
			&info_proxy::get_pool_sizes,
			&info_proxy::set_pool_sizes
		};
	};

	template<>
	struct info<DescriptorPool>
	{
		using handle_type = DescriptorPool;
		using base_info_type = DescriptorPoolCreateInfo;
		using type = info_proxy<DescriptorPoolCreateInfo>;
	};

	template<>
	struct info_proxy<DescriptorSetAllocateInfo> : info_proxy_base<DescriptorSetAllocateInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pSetLayouts)>> descriptor_set_layouts_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(descriptor_set_layouts_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(descriptor_set_layouts_)& get_descriptor_set_layouts() const noexcept;
		void set_descriptor_set_layouts(decltype(descriptor_set_layouts_));

		const property<info_proxy, decltype(descriptor_set_layouts_)> descriptor_set_layouts_property{
			*this,
			&info_proxy::get_descriptor_set_layouts,
			&info_proxy::set_descriptor_set_layouts
		};
	};

	template<>
	struct info<DescriptorSet>
	{
		using handle_type = DescriptorSet;
		using base_info_type = DescriptorSetAllocateInfo;
		using type = info_proxy<DescriptorSetAllocateInfo>;
	};

	template<>
	struct info_proxy<WriteDescriptorSet> : info_proxy_base<WriteDescriptorSet>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pImageInfo)>> image_infos_;
		vector<decay_to_origin_t<decltype(info.pBufferInfo)>> buffer_infos_;
		vector<decay_to_origin_t<decltype(info.pTexelBufferView)>> texel_buffer_views_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(image_infos_),
			decltype(buffer_infos_) = {},
			decltype(texel_buffer_views_) = {},
			decltype(info) = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(image_infos_)& get_image_infos() const noexcept;
		void set_image_infos(decltype(image_infos_));

		constexpr const decltype(buffer_infos_)& get_buffer_infos() const noexcept;
		void set_buffer_infos(decltype(buffer_infos_));

		constexpr const decltype(texel_buffer_views_)& get_texel_buffer_views() const noexcept;
		void set_texel_buffer_views(decltype(texel_buffer_views_));

		const property<info_proxy, decltype(image_infos_)> image_infos_property{
			*this,
			&info_proxy::get_image_infos,
			&info_proxy::set_image_infos
		};
		const property<info_proxy, decltype(buffer_infos_)> buffer_infos_property{
			*this,
			&info_proxy::get_buffer_infos,
			&info_proxy::set_buffer_infos
		};
		const property<info_proxy, decltype(texel_buffer_views_)> texel_buffer_views_property{
			*this,
			&info_proxy::get_texel_buffer_views,
			&info_proxy::set_texel_buffer_views
		};
	};

	template<>
	struct info_proxy<SubmitInfo> : info_proxy_base<SubmitInfo>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pWaitSemaphores)>> wait_semaphores_;
		optional<decay_to_origin_t<decltype(info.pWaitDstStageMask)>> dst_stage_mask_;
		vector<decay_to_origin_t<decltype(info.pCommandBuffers)>> command_buffers_;
		vector<decay_to_origin_t<decltype(info.pSignalSemaphores)>> signal_semaphores_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(wait_semaphores_),
			decltype(dst_stage_mask_) = {},
			decltype(command_buffers_) = {},
			decltype(signal_semaphores_) = {},
			decltype(info) info = {}
		);

		info_proxy(base_info_type = {});

		constexpr const decltype(wait_semaphores_)& get_wait_semaphores() const noexcept;
		void set_wait_semaphores(decltype(wait_semaphores_));

		constexpr const decltype(dst_stage_mask_)& get_dst_stage_mask() const noexcept;
		void set_dst_stage_mask(decltype(dst_stage_mask_));

		constexpr const decltype(command_buffers_)& get_command_buffers() const noexcept;
		void set_command_buffers(decltype(command_buffers_));

		constexpr const decltype(signal_semaphores_)& get_signal_semaphores() const noexcept;
		void set_signal_semaphores(decltype(signal_semaphores_));

		const property<info_proxy, decltype(wait_semaphores_)> wait_semaphores_property{
			*this,
			&info_proxy::get_wait_semaphores,
			&info_proxy::set_wait_semaphores
		};
		const property<info_proxy, decltype(dst_stage_mask_)> dst_stage_mask_property{
			*this,
			&info_proxy::get_dst_stage_mask,
			&info_proxy::set_dst_stage_mask
		};
		const property<info_proxy, decltype(command_buffers_)> command_buffers_property{
			*this,
			&info_proxy::get_command_buffers,
			&info_proxy::set_command_buffers
		};
		const property<info_proxy, decltype(signal_semaphores_)> signal_semaphores_property{
			*this,
			&info_proxy::get_signal_semaphores,
			&info_proxy::set_signal_semaphores
		};
	};

	template<>
	struct info_proxy<PresentInfoKHR> : info_proxy_base<PresentInfoKHR>
	{
		using base = info_proxy_base;

	private:
		vector<decay_to_origin_t<decltype(info.pWaitSemaphores)>> wait_semaphores_;
		vector<decay_to_origin_t<decltype(info.pSwapchains)>> swapchains_;
		vector<decay_to_origin_t<decltype(info.pImageIndices)>> image_indices_;
		vector<decay_to_origin_t<decltype(info.pResults)>> results_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(
			decltype(wait_semaphores_),
			decltype(swapchains_) = {},
			decltype(image_indices_) = {},
			decltype(results_) = {},
			decltype(info) = {}
		);
		info_proxy(base_info_type = {});

		constexpr const decltype(wait_semaphores_)& get_wait_semaphores() const noexcept;
		void set_wait_semaphores(decltype(wait_semaphores_));

		constexpr const decltype(swapchains_)& get_swapchains() const noexcept;
		void set_swapchains(decltype(swapchains_));

		constexpr const decltype(image_indices_)& get_image_indices() const noexcept;
		void set_image_indices(decltype(image_indices_));

		constexpr const decltype(results_)& get_results() const noexcept;
		void set_results(decltype(results_));

		const property<info_proxy, decltype(wait_semaphores_)> wait_semaphores_property{
			*this,
			&info_proxy::get_wait_semaphores,
			&info_proxy::set_wait_semaphores
		};
		const property<info_proxy, decltype(swapchains_)> swapchains_property{
			*this,
			&info_proxy::get_swapchains,
			&info_proxy::set_swapchains
		};
		const property<info_proxy, decltype(image_indices_)> image_indices_property{
			*this,
			&info_proxy::get_image_indices,
			&info_proxy::set_image_indices
		};
		const property<info_proxy, decltype(results_)> results_property{
			*this,
			&info_proxy::get_results,
			&info_proxy::set_results
		};
	};

	template<>
	struct info_proxy<BufferCreateInfo> : info_proxy_base<BufferCreateInfo>
	{
		using base = info_proxy_base<BufferCreateInfo>;

	private:
		vector<decay_to_origin_t<decltype(info.pQueueFamilyIndices)>> queue_family_indices_;
		set<std::decay_t<decltype(queue_family_indices_)::value_type>> queue_family_indices_set_;

		void property_copy(const info_proxy&) const;
		constexpr void property_move(info_proxy&&) const noexcept;

	public:
		DEFAULT_RULE_OF_5(info_proxy)

		explicit info_proxy(decltype(queue_family_indices_set_), decltype(info) = {});

		info_proxy(base_info_type = {});

		constexpr const decltype(queue_family_indices_set_)& get_queue_family_indices_set() const noexcept;
		void set_queue_family_indices_set(decltype(queue_family_indices_set_));

		const property<info_proxy, decltype(queue_family_indices_set_)> queue_family_indices_set_property{
			*this,
			&info_proxy::get_queue_family_indices_set,
			&info_proxy::set_queue_family_indices_set
		};
	};

	template<>
	struct info<Buffer>
	{
		using handle_type = Buffer;
		using base_info_type = BufferCreateInfo;
		using type = info_proxy<BufferCreateInfo>;
	};

	template<typename T>
	using info_base_t = typename info<T>::base_info_type;

	template<typename T>
	using info_t = typename info<T>::type;
}

#include "info.tpp"
