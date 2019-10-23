#pragma once

#include "vulkan_utility.h"
using namespace vulkan;

class vulkan_sample
{
	void initialize_window() noexcept;

	void generate_debug_messenger_create_info();
	void generate_instance_create_info();
	bool generate_physical_device(const PhysicalDevice& d);
	void generate_device_create_info();
	void generate_swap_chain_create_info();
	void generate_image_view_create_infos();
	void generate_render_pass_create_info();
	void generate_framebuffer_create_infos();
	void generate_shader_module_create_infos();
	void generate_descriptor_set_layout_create_info();
	void generate_descriptor_pool_create_info();
	void generate_descriptor_set_allocate_info();
	void generate_pipeline_layout_create_info();
	void generate_graphics_pipeline_create_info();
	void generate_transform_buffer_create_info();
	void generate_vertices_buffer_allocate_info();
	void generate_indices_buffer_allocate_info();
	void generate_vertices_staging_buffer_allocate_info();
	void generate_indices_staging_buffer_allocate_info();
	void generate_graphics_command_pool_create_info();
	void generate_graphics_command_buffer_allocate_info();
	void generate_sync_objects_create_info();
	void generate_render_info();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		const VkDebugUtilsMessageSeverityFlagBitsEXT,
		const VkDebugUtilsMessageTypeFlagsEXT,
		const VkDebugUtilsMessengerCallbackDataEXT*,
		void*
	);

	void initialize_vulkan();
	void re_initialize_vulkan();
	void glfw_cleanup() noexcept;

	GLFWwindow* window_{nullptr};

	instance instance_;

	debug_messenger debug_messenger_;

	surface surface_;

	decltype(DeviceQueueCreateInfo::queueFamilyIndex) graphics_queue_index_ = -1;
	decltype(DeviceQueueCreateInfo::queueFamilyIndex) present_queue_index_ = -1;

	PhysicalDevice physical_device_;

	device device_;

	Queue graphics_queue_;
	Queue present_queue_;

	swap_chain swap_chain_;

	vector<image_view> image_views_;

	render_pass render_pass_;

	vector<frame_buffer> frame_buffers_;

	shader_module vertex_shader_module_;
	shader_module fragment_shader_module_;

	descriptor_set_layout descriptor_set_layout_;
	descriptor_pool descriptor_pool_;
	vector<descriptor_set> descriptor_sets_;

	pipeline_layout pipeline_layout_;

	graphics_pipeline graphics_pipeline_;

	buffer vertices_buffer_;
	device_memory vertices_buffer_memory_;
	buffer vertices_staging_buffer_;
	device_memory vertices_staging_buffer_memory_;

	buffer indices_buffer_;
	device_memory indices_buffer_memory_;
	buffer indices_staging_buffer_;
	device_memory indices_staging_buffer_memory_;

	buffer transform_buffer_;
	device_memory transform_buffer_memory_;

	command_pool graphics_command_pool_;

	vector<command_buffer> graphics_command_buffers_;

	info_proxy<CommandBufferBeginInfo> command_buffer_begin_info_;
	vector<info_proxy<RenderPassBeginInfo>> render_pass_begin_infos_;
	vector<info_proxy<SubmitInfo>> submit_infos_;
	vector<info_proxy<PresentInfoKHR>> present_infos_;

	vector<semaphore> swap_chain_image_syn_;
	vector<semaphore> render_syn_;
	vector<fence> gpu_syn_;

	unsigned long long frame_count_ = 0;

public:
	vulkan_sample() = default;
	~vulkan_sample();

	const SwapchainCreateInfoKHR& swap_chain_create_info() const;

	void initialize();

	template<typename T>
	bool render(const T& t)
	{
		device_->waitIdle();

		if(glfwWindowShouldClose(window_))
			return false;
		glfwPollEvents();

		try
		{
			const auto& swap_chain_image_syn = *swap_chain_image_syn_[frame_count_ % swap_chain_image_syn_.size()];
			const auto index = device_->acquireNextImageKHR(
				*swap_chain_,
				-1,
				swap_chain_image_syn,
				nullptr
			).value;

			if constexpr(!std::is_same_v<T, empty_type>)
				t(index);

			submit_infos_[index].wait_semaphores_property = {swap_chain_image_syn};

			device_->waitForFences({*gpu_syn_[index]}, true, numberic_max<uint64_t>);
			device_->resetFences({*gpu_syn_[index]});

			graphics_queue_.submit({submit_infos_[index]}, *gpu_syn_[index]);

			present_queue_.presentKHR(present_infos_[index]);
		}
		catch(const SystemError & error)
		{
			if(error.code() == Result::eErrorOutOfDateKHR || error.code() == Result::eSuboptimalKHR)
			{
				re_initialize_vulkan();
				return true;
			}
			std::cerr << error.what();
			return false;
		}
		catch(const std::exception & e) { std::cerr << e.what(); return false; }

		++frame_count_;
		return true;
	}

	bool render();

	void flush_vertices_to_memory();
	void flush_indices_to_memory();
	void flush_transform_to_memory();
	void flush_to_memory();

	static constexpr uint32_t width = 800;
	static constexpr uint32_t height = 600;

	array<vertex, 4> vertices = {
		vertex{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		vertex{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		vertex{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		vertex{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	array<uint32_t, 6> indices = {0, 1, 2, 2, 3, 0};
	struct transform { mat4 mat; } transform{};
};