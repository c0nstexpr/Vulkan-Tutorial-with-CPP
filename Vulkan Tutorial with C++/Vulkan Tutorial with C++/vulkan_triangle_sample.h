#pragma once

#include "vulkan_utility.h"

using namespace vulkan;
constexpr bool is_debug
#ifdef _DEBUG
= true
#else
= false
#endif
;

class vulkan_triangle_sample
{
	void initialize_window() noexcept;

	void generate_debug_messenger_create_info();
	void generate_instance_create_info();
	bool generate_physical_device(const PhysicalDevice& d);
	void generate_device_create_info();
	void generate_swap_chain_create_info();
	void generate_image_view_create_infos();
	void generate_render_pass_create_info();
	void generate_shader_module_create_infos();
	void generate_pipeline_layout_create_info();
	void generate_graphics_pipeline_create_info();
	void generate_framebuffer_create_infos();
	void generate_graphics_command_pool_create_info();
	void generate_graphics_command_buffer_allocate_info();
	void generate_render_info();

	template<typename T>
	[[nodiscard]] static PhysicalDevice ini_physical_device(const Instance&, const T&);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT,
		VkDebugUtilsMessageTypeFlagsEXT,
		const VkDebugUtilsMessengerCallbackDataEXT*,
		void*
	);

	void initialize_vulkan();
	void re_initialize_vulkan();
	bool render();
	void main_loop() noexcept;
	void glfw_cleanup() noexcept;

	GLFWwindow* window_{nullptr};

	instance instance_;

	debug_messenger debug_messenger_;

	surface surface_;

	decltype(std::declval<DeviceQueueCreateInfo>().queueFamilyIndex) graphics_queue_index_ = -1;
	decltype(std::declval<DeviceQueueCreateInfo>().queueFamilyIndex) present_queue_index_ = -1;

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

	pipeline_layout pipeline_layout_;

	graphics_pipeline graphics_pipeline_;

	command_pool graphics_command_pool_;

	vector<command_buffer> graphics_command_buffers_;

	semaphore swapchain_image_syn_;
	semaphore render_syn_;

	info_proxy<CommandBufferBeginInfo> command_buffer_begin_info_;
	vector<info_proxy<RenderPassBeginInfo>> render_pass_begin_infos_;
	vector<info_proxy<SubmitInfo>> submit_infos_proxy_;
	vector<SubmitInfo> submit_infos_;
	vector<info_proxy<PresentInfoKHR>> present_infos_;

	fence gpu_syn_;

public:
	vulkan_triangle_sample() = default;
	~vulkan_triangle_sample();

	vulkan_triangle_sample(vulkan_triangle_sample&&) = default;
	vulkan_triangle_sample& operator=(vulkan_triangle_sample&&) noexcept = default;

	void run();

	[[nodiscard]] const auto& instance_create_info() const { return instance_.info; }
	[[nodiscard]] const auto& debug_messenger_create_info()const { return debug_messenger_.info; }
	[[nodiscard]] const auto& graphics_queue_create_info()const { return device_.info.queue_create_infos.at(graphics_queue_index_); }
	[[nodiscard]] const auto& present_queue_create_info()const { return device_.info.queue_create_infos.at(present_queue_index_); }
	[[nodiscard]] const auto& device_create_info() const { return device_.info.info; }
	[[nodiscard]] const auto& swap_chain_create_info()const { return swap_chain_.info.info; }
	[[nodiscard]] auto image_view_create_info() const
	{
		vector<image_view::info_type> infos(image_views_.size());
		std::generate(
			infos.begin(),
			infos.end(),
			[&, it = std::move(image_views_.cbegin())]()mutable  {return it++->info; }
		);
		return infos;
	}
	[[nodiscard]] const auto& render_pass_create_info()const { return render_pass_.info.info; }
	[[nodiscard]] const auto& vertex_shader_module_create_info()const { return vertex_shader_module_.info.info; }
	[[nodiscard]] const auto& frag_shader_module_create_info()const { return fragment_shader_module_.info.info; }
	[[nodiscard]] const auto& graphics_pipeline_create_infos()const { return graphics_pipeline_.info; }
	[[nodiscard]] auto framebuffer_infos()const
	{
		vector<frame_buffer::info_type> infos(frame_buffers_.size());
		std::generate(
			infos.begin(),
			infos.end(),
			[&, it = std::move(frame_buffers_.cbegin())]()mutable  {return it++->info; }
		);
		return infos;
	}
	[[nodiscard]] const auto& command_pool_create_info()const { return graphics_command_pool_.info; }
	[[nodiscard]] const auto& command_buffer_allocate_info()const
	{
		return graphics_command_buffers_.size() ? graphics_command_buffers_.front().info : command_buffer::info_type{};
	}

	static constexpr uint32_t width = 800;
	static constexpr uint32_t height = 600;
};
