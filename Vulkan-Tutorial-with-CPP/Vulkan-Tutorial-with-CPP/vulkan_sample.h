#pragma once

#include "vulkan/utility/utility.h"
#include "utility/constant/numberic.h"
#include "vulkan/utility/gltf/gltf.h"
#include <unordered_map>
#include <glm/gtx/hash.hpp>

namespace vulkan
{
    using namespace utility;
    using namespace constant;
    using namespace shaderc;

    using namespace ::utility::constant::numeric;
    using namespace ::utility::constant;
    using namespace ::utility;

    using ::time;
    using stb::channel;
    using std::unordered_map;

    class vulkan_sample
    {
        struct mesh
        {
            uint32_t first_index;
            uint32_t index_count;
            const texture_image<Format::eR8G8B8A8Unorm>* texture = nullptr;
            const  descriptor_set_object* descriptor_set = nullptr;
        };

        void initialize_window() noexcept;

        void generate_debug_messenger_create_info();
        void generate_instance_create_info();

        void initialize_instance();

        void generate_surface_create_info();

        void initialize_debug_messenger();
        void initialize_surface();

        bool generate_physical_device(const PhysicalDevice&, const surface_object&);
        void initialize_physical_device();

        void generate_device_create_info();
        void initialize_device();

        void initialize_queue();

        void generate_model();

        void generate_shader_module_create_infos();
        void generate_descriptor_set_layout_create_info();
        [[nodiscard]] map<string, stb::image<channel::rgb_alpha>> generate_texture_image_create_info();
        [[nodiscard]] pair<vector<vertex>, vector<uint32_t>> generate_buffer_allocate_info();
        void generate_texture_sampler_create_info();
        void generate_transform_buffer_create_info();
        void generate_graphics_command_pool_create_info();

        void initialize_shader_module();
        void initialize_descriptor_set_layout();
        void initialize_texture_image();
        void initialize_buffer();
        void initialize_texture_sampler();
        void initialize_transform_buffer();
        void initialize_graphics_command_pool();

        void generate_pipeline_layout_create_info(const descriptor_set_layout_object&);
        void generate_swapchain_create_info(const surface_object&);

        void initialize_pipeline_layout();
        void initialize_swapchain();

        void generate_depth_image_create_info(const swapchain_object&);
        void generate_image_view_create_infos(const swapchain_object&);

        void initialize_depth_image();
        void initialize_image_views();

        void generate_graphics_command_buffer_allocate_info(
            const vector<image_view_object>&,
            const command_pool_object&
        );
        void generate_render_pass_create_info(const swapchain_object&, const depth_image&);
        void generate_descriptor_pool_create_info(const vector<mesh>& meshes);
        void generate_sync_objects_create_info(const vector<image_view_object>&);

        void initialize_graphics_command_buffer();
        void initialize_render_pass();
        void initialize_descriptor_pool();
        void initialize_sync_objects();

        void generate_framebuffer_create_infos(
            const vector<image_view_object>&,
            const depth_image&,
            const render_pass_object&,
            const swapchain_object&
        );
        void generate_graphics_pipeline_create_info(
            const shader_module_object&,
            const shader_module_object&,
            const swapchain_object&,
            const render_pass_object&,
            const pipeline_layout_object&
        );
        void generate_descriptor_set_allocate_info(
            const vector<mesh>&,
            const descriptor_set_layout_object&,
            const descriptor_pool_object&
        );

        void initialize_frame_buffer();
        void initialize_graphics_pipeline();
        void initialize_descriptor_sets();

        void initialize_vulkan();

        void submit_precondition_command();
        void generate_render_info();
        void re_initialize_vulkan();
        void glfw_cleanup() noexcept;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
            const VkDebugUtilsMessageSeverityFlagBitsEXT,
            const VkDebugUtilsMessageTypeFlagsEXT,
            const VkDebugUtilsMessengerCallbackDataEXT*,
            void*
        );

        GLFWwindow* window_{nullptr};

        instance_object instance_;

        debug_messenger_object debug_messenger_;

        surface_object surface_;

        decltype(DeviceQueueCreateInfo::queueFamilyIndex) graphics_queue_index_ = queue_family_ignore<>;
        decltype(DeviceQueueCreateInfo::queueFamilyIndex) present_queue_index_ = queue_family_ignore<>;

        physical_device_object physical_device_;

        device_object device_;

        struct
        {
            tinyobj::attrib_t attribute;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
        }model_;

        vector<mesh> meshes_;

        Queue graphics_queue_;
        Queue present_queue_;

        swapchain_object swapchain_;

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
        static_memory<true,vertex, uint32_t>::vector_values transfer_memory_{};

        pipeline_layout_object pipeline_layout_;

        graphics_pipeline_object graphics_pipeline_;

        command_pool_object graphics_command_pool_;

        vector<command_buffer_object> graphics_command_buffers_;

        map<string,texture_image<Format::eR8G8B8A8Unorm>> texture_image_map_;

        sampler_object texture_sampler_;

        depth_image depth_image_;

        info_proxy<CommandBufferBeginInfo> command_buffer_begin_info_;
        vector<info_proxy<RenderPassBeginInfo>> render_pass_begin_infos_;
        vector<info_proxy<SubmitInfo>> submit_infos_;
        vector<info_proxy<PresentInfoKHR>> present_infos_;

        vector<semaphore_object> swapchain_image_syn_;
        vector<semaphore_object> render_syn_;
        vector<fence_object> gpu_syn_;

        struct transform { mat4 mat; } transform_mat_{};

        unsigned long long frame_count_ = 0;

        unsigned fps_ = 0;

    public:

        ~vulkan_sample();
        decltype(fps_) fps() const;

        const SwapchainCreateInfoKHR& swapchain_create_info() const;

        void initialize();

        template<typename T>
        bool render(const T&);

        bool render();

        void flush_vertices_to_memory();
        void flush_indices_to_memory();
        void flush_transform_to_memory();
        void flush_to_memory();

        void set_transform(decltype(transform_mat_));
        [[nodiscard]] constexpr const decltype(transform_mat_)& get_transform() const;

        void set_vertices(decltype(transfer_memory_)::value_type<vertex>);
        [[nodiscard]] constexpr const decltype(transfer_memory_)::value_type<vertex>& get_vertices() const;

        void set_indices(decltype(transfer_memory_)::value_type<uint32_t>);
        [[nodiscard]] constexpr const decltype(transfer_memory_)::value_type<uint32_t>& get_indices() const;

        [[nodiscard]] constexpr const decltype(window_)& get_window() const;

        static constexpr uint32_t width = 1280;
        static constexpr uint32_t height = 960;
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