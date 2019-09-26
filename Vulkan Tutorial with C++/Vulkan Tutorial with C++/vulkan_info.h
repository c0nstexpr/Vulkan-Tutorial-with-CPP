#pragma once

#include"utility.h"
#include <algorithm>

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

	struct VertexBuffer
	{
		using handle_type = Buffer;
	};

	struct VertexBufferCreateInfo : BufferCreateInfo
	{
		VertexBufferCreateInfo(
			const BufferCreateFlags flags_ = BufferCreateFlags(),
			const DeviceSize size_ = 0,
			const SharingMode sharingMode_ = SharingMode::eExclusive,
			const uint32_t queueFamilyIndexCount_ = 0,
			const uint32_t * pQueueFamilyIndices_ = nullptr)
			: BufferCreateInfo(
				flags_,
				size_,
				BufferUsageFlagBits::eVertexBuffer,
				sharingMode_,
				queueFamilyIndexCount_,
				pQueueFamilyIndices_
			)
		{}
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
	struct info<VertexBuffer>
	{
		using handle_type = VertexBuffer::handle_type;
		using type = VertexBufferCreateInfo;
	};

	template<>
	struct info<DeviceMemory>
	{
		using handle_type = DeviceMemory;
		using type = MemoryAllocateInfo;
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

		info_proxy() = default;

		explicit info_proxy(
			decltype(application_name),
			decltype(engine_name) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&) noexcept;
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<InstanceCreateInfo>
	{
		using info_type = InstanceCreateInfo;

		info_type info;
		optional<info_proxy<decay_to_origin_t<decltype(info.pApplicationInfo)>>> application_info;
		vector<string> extension_names;
		vector<string> layer_names;

		info_proxy() = default;

		explicit info_proxy(
			decltype(application_info),
			decltype(extension_names) = {},
			decltype(layer_names) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(decltype(priorities), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(
			decltype(queue_create_infos),
			decltype(extension_names) = {},
			decltype(features) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(decltype(queue_family_indices), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(
			decltype(input_attachments),
			decltype(color_attachments) = {},
			decltype(resolve_attachments) = {},
			decltype(depth_attachment) = {},
			decltype(preserve_attachments) = {},
			decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(
			decltype(attachment_descriptions),
			decltype(subpass_descriptions) = {},
			decltype(dependencies) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(decltype(image_views), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;
		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(decltype(codes), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(decltype(map_entries), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<PipelineShaderStageCreateInfo>
	{
		using info_type = PipelineShaderStageCreateInfo;

		info_type info;
		string entry_name;
		optional<info_proxy<decay_to_origin_t<decltype(info.pSpecializationInfo)>>> specialization_info;

		info_proxy() = default;

		explicit info_proxy(
			decltype(entry_name),
			decltype(specialization_info) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;
		operator info_type& ();
		operator const info_type& () const;
		void set_proxy();
	};

	template<>
	struct info_proxy<PipelineVertexInputStateCreateInfo>
	{
		using info_type = PipelineVertexInputStateCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pVertexBindingDescriptions)>> vertex_input_binding_descriptions;
		vector<decay_to_origin_t<decltype(info.pVertexAttributeDescriptions)>> vertex_input_attribute_descriptions;

		info_proxy() = default;

		explicit info_proxy(
			decltype(vertex_input_binding_descriptions),
			decltype(vertex_input_attribute_descriptions) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<PipelineViewportStateCreateInfo>
	{
		using info_type = PipelineViewportStateCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pViewports)>> viewports;
		vector<decay_to_origin_t<decltype(info.pScissors)>> scissors;

		info_proxy() = default;

		explicit info_proxy(decltype(viewports), decltype(scissors) = {}, decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<PipelineColorBlendStateCreateInfo>
	{
		using info_type = PipelineColorBlendStateCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pAttachments)>> attachment_states;

		info_proxy() = default;

		explicit info_proxy(decltype(attachment_states), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<PipelineLayoutCreateInfo>
	{
		using info_type = PipelineLayoutCreateInfo;

		info_type info;
		vector<decay_to_origin_t<decltype(info.pSetLayouts)>> set_layouts;
		vector<decay_to_origin_t<decltype(info.pPushConstantRanges)>> push_const_ranges;

		info_proxy() = default;

		explicit info_proxy(
			decltype(set_layouts),
			decltype(push_const_ranges) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(
			decltype(stages_create_infos),
			decltype(vertex_input_state) = {},
			decltype(input_assembly_state) = {},
			decltype(tessellation_state) = {},
			decltype(viewport_state) = {},
			decltype(rasterization_state_create_info) = {},
			decltype(multi_sample_state_create_info) = {},
			decltype(depth_stencil_state_create_info) = {},
			decltype(color_blend_state_create_info) = {},
			decltype(dynamic_state_create_info) = {},
			decltype(info) = {}
		);

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info<GraphicsPipeline>
	{
		using handle_type = GraphicsPipeline::handle_type;
		using type = info_proxy<GraphicsPipelineCreateInfo>;
	};

	template<>
	struct info_proxy<CommandBufferBeginInfo>
	{
		using info_type = CommandBufferBeginInfo;

		info_type info;
		optional<decay_to_origin_t<decltype(info.pInheritanceInfo)>> inheritance_info;

		info_proxy() = default;

		explicit info_proxy(decltype(inheritance_info), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<RenderPassBeginInfo>
	{
		using info_type = RenderPassBeginInfo;

		info_type info;

		vector<decay_to_origin_t<decltype(info.pClearValues)>> clear_values;

		info_proxy() = default;

		explicit info_proxy(decltype(clear_values), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<>
	struct info_proxy<DescriptorSetAllocateInfo>
	{
		using info_type = DescriptorSetAllocateInfo;

		info_type info;

		vector<decay_to_origin_t<decltype(info.pSetLayouts)>> descriptor_set_layouts;

		info_proxy() = default;

		explicit info_proxy(decltype(descriptor_set_layouts), decltype(info) = {});

		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		vector<decay_to_origin_t<decltype(info.pWaitSemaphores)>> wait_semaphores;
		optional<decay_to_origin_t<decltype(info.pWaitDstStageMask)>> dst_stage_mask;
		vector<decay_to_origin_t<decltype(info.pCommandBuffers)>> command_buffers;
		vector<decay_to_origin_t<decltype(info.pSignalSemaphores)>> signal_semaphores;

		info_proxy() = default;

		explicit info_proxy(
			decltype(wait_semaphores),
			decltype(dst_stage_mask) = {},
			decltype(command_buffers) = {},
			decltype(signal_semaphores) = {},
			decltype(info) info = {}
		);
		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
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

		info_proxy() = default;

		explicit info_proxy(
			decltype(semaphores),
			decltype(swap_chains) = {},
			decltype(image_indices) = {},
			decltype(results) = {},
			decltype(info) = {}
		);
		info_proxy(info_type);

		info_proxy(const info_proxy&);
		info_proxy(info_proxy&&) noexcept;
		info_proxy& operator=(const info_proxy&);
		info_proxy& operator=(info_proxy&&) noexcept;

		operator info_type& ();
		operator const info_type& () const;

		void set_proxy();
	};

	template<typename T>
	using info_t = typename info<T>::type;
}
