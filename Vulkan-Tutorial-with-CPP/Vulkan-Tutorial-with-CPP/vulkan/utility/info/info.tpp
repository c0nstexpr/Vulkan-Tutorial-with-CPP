#pragma once

namespace vulkan::utility
{
	constexpr void info_proxy<ApplicationInfo>::property_move(info_proxy&& right) const noexcept
	{
		application_name_property = std::move(right.application_name_);
		engine_name_property = std::move(right.engine_name_);
	}

	constexpr auto info_proxy<ApplicationInfo>::get_application_name() const noexcept ->
		const decltype(application_name_)& { return application_name_; }

	constexpr auto info_proxy<ApplicationInfo>::get_engine_name() const noexcept ->
		const decltype(engine_name_)& { return engine_name_; }

	constexpr void info_proxy<InstanceCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		application_info_property = std::move(right.application_info_);
		extension_name_strs_property = std::move(right.extension_name_strs_);
		layer_name_strs_property = std::move(right.layer_name_strs_);
	}

	constexpr auto info_proxy<InstanceCreateInfo>::get_application_info() const noexcept ->
		const decltype(application_info_)& { return application_info_; }

	constexpr auto info_proxy<InstanceCreateInfo>::get_extension_name_strs() const noexcept ->
		const decltype(extension_name_strs_)& {return extension_name_strs_;	}

	constexpr auto info_proxy<InstanceCreateInfo>::get_layer_name_strs() const noexcept ->
		const decltype(layer_name_strs_)& { return layer_name_strs_; }

	constexpr void info_proxy<DeviceQueueCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		priorities_property = std::move(right.priorities_);
	}

	constexpr auto info_proxy<DeviceQueueCreateInfo>::get_priorities() const noexcept ->
		const decltype(priorities_)& { return priorities_; }

	constexpr void info_proxy<DeviceCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		queue_create_infos_set_property = std::move(right.queue_create_infos_set_);
		extension_name_strs_property = std::move(right.extension_name_strs_);
		features_property = std::move(right.features_);
	}

	constexpr auto info_proxy<DeviceCreateInfo>::get_extension_name_strs() const noexcept ->
		const decltype(extension_name_strs_)& { return extension_name_strs_; }

	constexpr auto info_proxy<DeviceCreateInfo>::get_queue_create_info_set() const noexcept ->
		const decltype(queue_create_infos_set_)& { return queue_create_infos_set_; }

	constexpr auto info_proxy<DeviceCreateInfo>::get_features() const noexcept ->
		const decltype(features_)& { return features_; }

	constexpr void info_proxy<SwapchainCreateInfoKHR>::property_move(info_proxy&& right) const noexcept
	{
		queue_family_indices_set_property = std::move(right.queue_family_indices_set_);
	}

	constexpr auto info_proxy<SwapchainCreateInfoKHR>::get_queue_family_indices_set() const noexcept ->
		const decltype(queue_family_indices_set_)& { return queue_family_indices_set_; }

	constexpr void info_proxy<SubpassDescription>::property_move(info_proxy&& right) const noexcept
	{
		input_attachments_property = std::move(right.input_attachments_);
		color_attachments_property = std::move(right.color_attachments_);
		resolve_attachments_property = std::move(right.resolve_attachments_);
		depth_attachment_property = std::move(right.depth_attachment_);
		preserve_attachments_property = std::move(right.preserve_attachments_);
	}

	constexpr auto info_proxy<SubpassDescription>::get_input_attachments() const noexcept ->
		const decltype(input_attachments_)& { return input_attachments_; }

	constexpr auto info_proxy<SubpassDescription>::get_color_attachments() const noexcept ->
		const decltype(color_attachments_)& { return color_attachments_; }

	constexpr auto info_proxy<SubpassDescription>::get_resolve_attachments() const noexcept ->
		const decltype(resolve_attachments_)& { return resolve_attachments_; }

	constexpr auto info_proxy<SubpassDescription>::get_depth_attachment() const noexcept ->
		const decltype(depth_attachment_)& { return depth_attachment_; }

	constexpr auto info_proxy<SubpassDescription>::get_preserve_attachments() const noexcept ->
		const decltype(preserve_attachments_)& { return preserve_attachments_; }

	constexpr void info_proxy<RenderPassCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		attachment_descriptions_property = std::move(right.attachment_descriptions_);
		subpass_descriptions_info_proxies_property = std::move(right.subpass_descriptions_info_proxies_);
		dependencies_property = std::move(right.dependencies_);
	}

	constexpr auto info_proxy<RenderPassCreateInfo>::get_attachment_descriptions() const noexcept ->
		const decltype(attachment_descriptions_)& { return attachment_descriptions_; }

	constexpr auto info_proxy<RenderPassCreateInfo>::get_subpass_descriptions_info_proxies() const noexcept ->
		const decltype(subpass_descriptions_info_proxies_)& { return subpass_descriptions_info_proxies_; }

	constexpr auto info_proxy<RenderPassCreateInfo>::get_dependencies() const noexcept ->
		const decltype(dependencies_)& { return dependencies_; }

	constexpr void info_proxy<FramebufferCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		image_views_property = std::move(right.image_views_);
	}

	constexpr auto info_proxy<FramebufferCreateInfo>::get_image_views() const noexcept ->
		const decltype(image_views_)& { return image_views_; }

	constexpr void info_proxy<ShaderModuleCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		codes_property = std::move(right.codes_);
	}

	constexpr auto info_proxy<ShaderModuleCreateInfo>::get_codes() const noexcept ->
		const decltype(codes_)& { return codes_; }

	constexpr void info_proxy<ImageCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		queue_family_indices_set_property = std::move(right.queue_family_indices_set_);
	}

	constexpr auto info_proxy<ImageCreateInfo>::get_queue_family_indices_set() const noexcept ->
		const decltype(queue_family_indices_set_)& { return queue_family_indices_set_; }

	constexpr void info_proxy<SpecializationInfo>::property_move(info_proxy&& right) const noexcept
	{
		map_entries_property = std::move(right.map_entries_);
	}

	constexpr auto info_proxy<SpecializationInfo>::get_map_entries() const noexcept ->
		const decltype(map_entries_)& { return map_entries_; }

	constexpr void info_proxy<PipelineShaderStageCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		entry_name_property = std::move(right.entry_name_);
		specialization_info_property = std::move(right.specialization_info_);
	}

	constexpr auto info_proxy<PipelineShaderStageCreateInfo>::get_entry_name() const noexcept ->
		const decltype(entry_name_)& { return entry_name_; }

	constexpr auto info_proxy<PipelineShaderStageCreateInfo>::get_specialization_info() const noexcept ->
		const decltype(specialization_info_)& { return specialization_info_; }

	constexpr void
		info_proxy<PipelineVertexInputStateCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		vertex_input_binding_descriptions_property = std::move(right.vertex_input_binding_descriptions_);
		vertex_input_attribute_descriptions_property = std::move(right.vertex_input_attribute_descriptions_);
	}

	constexpr auto
		info_proxy<PipelineVertexInputStateCreateInfo>::get_vertex_input_binding_descriptions() const noexcept ->
		const decltype(vertex_input_binding_descriptions_)& { return vertex_input_binding_descriptions_; }

	constexpr auto
		info_proxy<PipelineVertexInputStateCreateInfo>::get_vertex_input_attribute_descriptions() const noexcept ->
		const decltype(vertex_input_attribute_descriptions_)& { return vertex_input_attribute_descriptions_; }

	constexpr void info_proxy<PipelineViewportStateCreateInfo>::property_move(info_proxy&& right) const noexcept

	{
		viewports_property = std::move(right.viewports_);
		scissors_property = std::move(right.scissors_);
	}

	constexpr auto info_proxy<PipelineViewportStateCreateInfo>::get_viewports() const noexcept ->
		const decltype(viewports_)& { return viewports_; }

	constexpr auto info_proxy<PipelineViewportStateCreateInfo>::get_scissors() const noexcept ->
		const decltype(scissors_)& { return scissors_; }

	constexpr void
		info_proxy<PipelineColorBlendStateCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		attachment_states_property = std::move(right.attachment_states_);
	}

	constexpr auto info_proxy<PipelineColorBlendStateCreateInfo>::get_attachment_states() const noexcept ->
		const decltype(attachment_states_)& {
		return attachment_states_;
	}

	constexpr void info_proxy<PipelineLayoutCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		set_layouts_property = std::move(right.set_layouts_);
		push_const_ranges_property = std::move(right.push_const_ranges_);
	}

	constexpr auto info_proxy<PipelineLayoutCreateInfo>::get_set_layouts() const noexcept ->
		const decltype(set_layouts_)& { return set_layouts_; }

	constexpr auto info_proxy<PipelineLayoutCreateInfo>::get_push_const_ranges() const noexcept ->
		const decltype(push_const_ranges_)& { return push_const_ranges_; }

	constexpr void info_proxy<graphics_pipeline_create_info>::property_move(info_proxy&& right) const noexcept
	{
		stages_info_proxies_property = std::move(right.stage_info_proxies_);
		vertex_input_state_property = std::move(right.vertex_input_state_);
		input_assembly_state_property = std::move(right.input_assembly_state_);
		tessellation_state_property = std::move(right.tessellation_state_);
		viewport_state_property = std::move(right.viewport_state_);
		rasterization_state_property = std::move(right.rasterization_state_);
		multi_sample_state_property = std::move(right.multi_sample_state_);
		depth_stencil_state_property = std::move(right.depth_stencil_state_);
		color_blend_state_property = std::move(right.color_blend_state_);
		dynamic_state_property = std::move(right.dynamic_state_);
	}

	constexpr auto info_proxy<graphics_pipeline_create_info>::
		get_stage_info_proxies() const noexcept -> const decltype(stage_info_proxies_)& {
		return stage_info_proxies_;
	}

	constexpr auto info_proxy<graphics_pipeline_create_info>::
		get_vertex_input_state() const noexcept -> const decltype(vertex_input_state_)& {
		return vertex_input_state_;
	}

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_input_assembly_state() const noexcept ->
		const decltype(input_assembly_state_)& { return input_assembly_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_tessellation_state() const noexcept ->
		const  decltype(tessellation_state_)& { return tessellation_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_viewport_state() const noexcept ->
		const decltype(viewport_state_)& { return viewport_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_rasterization_state() const noexcept ->
		const decltype(rasterization_state_)& { return rasterization_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_multi_sample_state() const noexcept ->
		const decltype(multi_sample_state_)& { return multi_sample_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_depth_stencil_state() const noexcept ->
		const decltype(depth_stencil_state_)& { return depth_stencil_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_color_blend_state() const noexcept ->
		const decltype(color_blend_state_)& { return color_blend_state_; }

	constexpr auto info_proxy<graphics_pipeline_create_info>::get_dynamic_state() const noexcept ->
		const decltype(dynamic_state_)& { return dynamic_state_; }

	constexpr void info_proxy<CommandBufferBeginInfo>::property_move(info_proxy&& right) const noexcept
	{
		inheritance_info_property = std::move(right.inheritance_info_);
	}

	constexpr auto info_proxy<CommandBufferBeginInfo>::get_inheritance_info() const noexcept ->
		const decltype(inheritance_info_)& { return inheritance_info_; }

	constexpr void info_proxy<RenderPassBeginInfo>::property_move(info_proxy&& right) const noexcept
	{
		clear_values_property = std::move(right.clear_values_);
	}

	constexpr auto info_proxy<RenderPassBeginInfo>::get_clear_values() const noexcept ->
		const decltype(clear_values_)& { return clear_values_; }

	constexpr void info_proxy<DescriptorSetLayoutBinding>::property_move(info_proxy&& right) const noexcept
	{
		samplers_property = std::move(right.samplers_);
	}

	constexpr auto info_proxy<DescriptorSetLayoutBinding>::get_samplers() const noexcept ->
		const decltype(samplers_)& { return samplers_; }

	constexpr void info_proxy<DescriptorSetLayoutCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		bindings_proxies_property = std::move(right.bindings_proxies_);
	}

	constexpr auto info_proxy<DescriptorSetLayoutCreateInfo>::get_binding_proxies() const noexcept ->
		const decltype(bindings_proxies_)& { return bindings_proxies_; }

	constexpr void info_proxy<DescriptorPoolCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		pool_sizes_property = std::move(right.pool_sizes_);
	}

	constexpr auto info_proxy<DescriptorPoolCreateInfo>::get_pool_sizes() const noexcept ->
		const decltype(pool_sizes_)& { return pool_sizes_; }

	constexpr void info_proxy<DescriptorSetAllocateInfo>::property_move(info_proxy&& right) const noexcept
	{
		descriptor_set_layouts_property = std::move(right.descriptor_set_layouts_);
	}

	constexpr auto info_proxy<DescriptorSetAllocateInfo>::get_descriptor_set_layouts() const noexcept ->
		const decltype(descriptor_set_layouts_)& { return descriptor_set_layouts_; }

	constexpr void info_proxy<WriteDescriptorSet>::property_move(info_proxy&& right) const noexcept
	{
		image_infos_property = std::move(right.image_infos_);
		buffer_infos_property = std::move(right.buffer_infos_);
		texel_buffer_views_property = std::move(right.texel_buffer_views_);
	}

	constexpr auto info_proxy<WriteDescriptorSet>::get_image_infos() const noexcept ->
		const decltype(image_infos_)& { return image_infos_; }

	constexpr auto info_proxy<WriteDescriptorSet>::get_buffer_infos() const noexcept ->
		const decltype(buffer_infos_)& { return buffer_infos_; }

	constexpr auto info_proxy<WriteDescriptorSet>::get_texel_buffer_views() const noexcept ->
		const decltype(texel_buffer_views_)& { return texel_buffer_views_; }

	constexpr void info_proxy<SubmitInfo>::property_move(info_proxy&& right) const noexcept
	{
		wait_semaphores_property = std::move(right.wait_semaphores_);
		dst_stage_mask_property = std::move(right.dst_stage_mask_);
		command_buffers_property = std::move(right.command_buffers_);
		signal_semaphores_property = std::move(right.signal_semaphores_);
	}

	constexpr auto info_proxy<SubmitInfo>::get_wait_semaphores() const noexcept ->
		const decltype(wait_semaphores_)& { return wait_semaphores_; }

	constexpr auto info_proxy<SubmitInfo>::get_dst_stage_mask() const noexcept ->
		const decltype(dst_stage_mask_)& { return dst_stage_mask_; }

	constexpr auto info_proxy<SubmitInfo>::get_command_buffers() const noexcept ->
		const decltype(command_buffers_)& { return command_buffers_; }

	constexpr auto info_proxy<SubmitInfo>::get_signal_semaphores() const noexcept ->
		const decltype(signal_semaphores_)& { return signal_semaphores_; }

	constexpr void info_proxy<PresentInfoKHR>::property_move(info_proxy&& right) const noexcept
	{
		wait_semaphores_property = std::move(right.wait_semaphores_);
		swap_chains_property = std::move(right.swap_chains_);
		image_indices_property = std::move(right.image_indices_);
		results_property = std::move(right.results_);
	}

	constexpr auto info_proxy<PresentInfoKHR>::get_wait_semaphores() const noexcept ->
		const decltype(wait_semaphores_)& { return wait_semaphores_; }

	constexpr auto info_proxy<PresentInfoKHR>::get_swap_chains() const noexcept ->
		const decltype(swap_chains_)& { return swap_chains_; }

	constexpr auto info_proxy<PresentInfoKHR>::get_image_indices() const noexcept ->
		const decltype(image_indices_)& { return image_indices_; }

	constexpr auto info_proxy<PresentInfoKHR>::get_results() const noexcept ->
		const decltype(results_)& { return results_; }

	constexpr void info_proxy<BufferCreateInfo>::property_move(info_proxy&& right) const noexcept
	{
		queue_family_indices_set_property = std::move(right.queue_family_indices_set_);
	}

	constexpr auto info_proxy<BufferCreateInfo>::get_queue_family_indices_set() const noexcept ->
		const decltype(queue_family_indices_set_)& { return queue_family_indices_set_; }
}