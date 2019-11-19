#pragma once

#include "vulkan/utility/utility.h"
#include "utility/constant/numberic.h"

namespace vulkan
{
    using namespace utility;
    using namespace ::utility::constant;
    using namespace ::utility;
    using namespace constant;
    using namespace numeric;
    using namespace shaderc;
    using ::time;
    using stb::channel;

    class vulkan_sample
    {
        void initialize_window() noexcept;

        void generate_debug_messenger_create_info();
        void generate_instance_create_info();
        bool generate_physical_device(const PhysicalDevice&);
        void generate_device_create_info();
        void generate_surface_create_info();
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
        void generate_graphics_command_pool_create_info();
        void generate_graphics_command_buffer_allocate_info();
        void generate_buffer_allocate_info();
        void generate_image_create_info();
        void generate_sync_objects_create_info();
        void submit_precondition_command();
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

        instance_object instance_;

        debug_messenger_object debug_messenger_;

        surface_object surface_;

        decltype(DeviceQueueCreateInfo::queueFamilyIndex) graphics_queue_index_ = queue_family_ignore<>;
        decltype(DeviceQueueCreateInfo::queueFamilyIndex) present_queue_index_ = queue_family_ignore<>;

        physical_device_object physical_device_;

        device_object device_;

        Queue graphics_queue_;
        Queue present_queue_;

        swap_chain_object swap_chain_;

        vector<image_view_object> image_views_;

        render_pass_object render_pass_;

        vector<frame_buffer_object> frame_buffers_;

        shader_module_object vertex_shader_module_;
        shader_module_object fragment_shader_module_;

        descriptor_set_layout_object descriptor_set_layout_;
        descriptor_pool_object descriptor_pool_;
        vector<descriptor_set_object> descriptor_sets_;
        buffer_object transform_buffer_;
        device_memory_object transform_buffer_memory_;

        static constexpr size_t vertices_buffer_index = 0;
        static constexpr size_t indices_buffer_index = 1;
        memory<true, vertex, uint32_t>::array_values<4, 6> transfer_memory_;

        pipeline_layout_object pipeline_layout_;

        graphics_pipeline_object graphics_pipeline_;

        command_pool_object graphics_command_pool_;

        vector<command_buffer_object> graphics_command_buffers_;

        stb::image<channel::rgb_alpha> texture_image_;
        buffer_object image_buffer_;
        device_memory_object image_buffer_memory_;
        image_object image_;
        device_memory_object image_memory_;

        info_proxy<CommandBufferBeginInfo> command_buffer_begin_info_;
        vector<info_proxy<RenderPassBeginInfo>> render_pass_begin_infos_;
        vector<info_proxy<SubmitInfo>> submit_infos_;
        vector<info_proxy<PresentInfoKHR>> present_infos_;

        vector<semaphore_object> swap_chain_image_syn_;
        vector<semaphore_object> render_syn_;
        vector<fence_object> gpu_syn_;

        struct transform { mat4 mat; } transform_mat_{};

        unsigned long long frame_count_ = 0;

        unsigned fps_ = 0;

    public:

        ~vulkan_sample();
        decltype(fps_) fps() const;

        const SwapchainCreateInfoKHR& swap_chain_create_info() const;

        void initialize();

        template<typename T>
        bool render(const T&);

        bool render();

        void flush_vertices_to_memory();
        void flush_indices_to_memory();
        void flush_transform_to_memory();
        void flush_to_memory();

        void set_transform(decltype(transform_mat_));
        constexpr const decltype(transform_mat_)& get_transform() const;

        void set_vertices(decltype(transfer_memory_)::value_type<vertex>);
        constexpr const decltype(transfer_memory_)::value_type<vertex>& get_vertices() const;

        void set_indices(decltype(transfer_memory_)::value_type<uint32_t>);
        constexpr const decltype(transfer_memory_)::value_type<uint32_t>& get_indices() const;

        void set_image(decltype(texture_image_));

        static constexpr uint32_t width = 800;
        static constexpr uint32_t height = 600;
        static const string window_title;

        const property<vulkan_sample, decltype(transform_mat_)> transform_property{
            *this,
                & vulkan_sample::get_transform,
                & vulkan_sample::set_transform
        };

        const property<vulkan_sample, std::decay_t<decltype(transfer_memory_.read<vertex>())>> vertices_property{
            *this,
                & vulkan_sample::get_vertices,
                & vulkan_sample::set_vertices
        };
        const property<vulkan_sample, std::decay_t<decltype(transfer_memory_.read<uint32_t >())>> indices_property{
            *this,
                & vulkan_sample::get_indices,
                & vulkan_sample::set_indices
        };
    };
}

#include "vulkan_sample.tpp"