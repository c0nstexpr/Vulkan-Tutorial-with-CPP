#pragma once

#include "vulkan/utility/constant/constant.h"
#include "object_traits.h"

namespace vulkan::utility
{
    using std::array;
    using glm::vec2;
    using glm::vec3;

    // ReSharper disable CppFunctionIsNotImplemented
    template<typename HandleType>
    class object : public object_traits<HandleType>
    {
    public:
        using base = object_traits<HandleType>;
        using base::base;

        using handle_type = typename base::handle_type;
        using info_type = typename base::info_type;
        using base_info_type = typename base::base_info_type;
        using owner_type = decltype(base::default_deleter_type{}.getOwner());
        using dispatch_type = typename base::dispatch_type;

        // Specializations implemented
        [[nodiscard]] static typename base::base create_unique_handle(
            const owner_type&,
            const dispatch_type&,
            const base_info_type&,
            const optional<AllocationCallbacks> & = nullopt
        );

        [[nodiscard]] static vector<typename base::base> create_unique_handles(
            const owner_type&,
            const dispatch_type&,
            vector<base_info_type>&,
            const optional<AllocationCallbacks> & = nullopt
        );

        object(info_type, const owner_type&, const dispatch_type&, const optional<AllocationCallbacks> & = nullopt);
        object(info_type, const object<owner_type>&, const optional<AllocationCallbacks> & = nullopt);

        void initialize(const owner_type&, const dispatch_type&, const optional<AllocationCallbacks> & = nullopt);
        void initialize(const object<owner_type>& owner_object, const optional<AllocationCallbacks> & = nullopt);
    };

    template<>
    class object<PhysicalDevice>
    {
    public:
        using handle_type = PhysicalDevice;

    private:
        handle_type physical_device_;

    public:
        constexpr object(std::nullptr_t = {}) noexcept;

        template<typename T>
        constexpr object(const Instance&, const T&) noexcept(std::is_nothrow_invocable_v<T, PhysicalDevice>);

        constexpr handle_type& operator*() noexcept;
        constexpr const handle_type& operator*() const noexcept;
        constexpr handle_type* operator->() noexcept;
        constexpr const handle_type* operator->() const noexcept;
    };

    template<>
    class object<Instance> : public object_traits<Instance, DispatchLoaderStatic>
    {
        DispatchLoaderDynamic dispatch_{get_instance_proc_addr};

    public:
        using base = object_traits<Instance, DispatchLoaderStatic>;
        using base::base;

        [[nodiscard]] static base::base create_unique_handle(
            const base_info_type & = {},
            const optional<AllocationCallbacks> & = nullopt
        );

        void initialize(const optional<AllocationCallbacks> & = nullopt);

        constexpr const DispatchLoaderDynamic& dispatch() const noexcept;
    };

    template<>
    class object<Device> : public object_traits<Device, DispatchLoaderStatic>
    {
        DispatchLoaderDynamic dispatch_{get_instance_proc_addr};

    public:
        using base = object_traits<Device, DispatchLoaderStatic>;
        using base::base;

        [[nodiscard]] static base::base create_unique_handle(
            const PhysicalDevice&,
            const base_info_type & = {},
            const optional<AllocationCallbacks> & = nullopt
        );

        void initialize(const PhysicalDevice&, const DispatchLoaderDynamic&, const optional<AllocationCallbacks> & = nullopt);

        constexpr const DispatchLoaderDynamic& dispatch() const noexcept;
    };

    template<typename HandleType>
    class pool_object : public object<HandleType>
    {
    public:
        using base = object<HandleType>;
        using base::base;

        using handle_type = typename base::handle_type;
        using info_type = typename base::info_type;
        using base_info_type = typename base::base_info_type;
        using owner_type = decltype(base::default_deleter_type{}.getOwner());
        using dispatch_type = typename base::dispatch_type;

        using element_handle_t = typename base::base::pool::element_type;

        // Specializations implemented
        [[nodiscard]] static auto create_element_unique_handles(
            const owner_type&,
            const dispatch_type&,
            const typename info<element_handle_t>::base_info_type & = {}
        )->vector<UniqueHandle<element_handle_t, DispatchLoaderDynamic>>;

        [[nodiscard]] static vector<object<element_handle_t>> create_element_objects(
            const owner_type&,
            const dispatch_type&,
            const typename info<element_handle_t>::base_info_type & = {}
        );

        [[nodiscard]] static vector<object<element_handle_t>> create_element_objects(
            const object<owner_type>&,
            const typename info<element_handle_t>::base_info_type&
        );

        template<typename PoolObjectRangeType>
        void element_objects_reset(const object<owner_type>&, PoolObjectRangeType&);
    };

    // ReSharper restore CppFunctionIsNotImplemented

    using instance_object = object<Instance>;
    using device_object = object<Device>;
    using physical_device_object = object<PhysicalDevice>;
    using debug_messenger_object = object<DebugUtilsMessengerEXT>;
    using surface_object = object<SurfaceKHR>;
    using swapchain_object = object<SwapchainKHR>;
    using image_view_object = object<ImageView>;
    using render_pass_object = object<RenderPass>;
    using frame_buffer_object = object<Framebuffer>;
    using shader_module_object = object<ShaderModule>;
    using buffer_object = object<Buffer>;
    using image_object = object<Image>;
    using sampler_object = object<Sampler>;
    using descriptor_set_layout_object = object<DescriptorSetLayout>;
    using descriptor_pool_object = pool_object<DescriptorPool>;
    using descriptor_set_object = object<DescriptorSet>;
    using pipeline_layout_object = object<PipelineLayout>;
    using device_memory_object = object<DeviceMemory>;
    using graphics_pipeline_object = object<GraphicsPipeline>;
    using command_pool_object = pool_object<CommandPool>;
    using command_buffer_object = object<CommandBuffer>;
    using semaphore_object = object<Semaphore>;
    using fence_object = object<Fence>;

    struct vertex_base
    {
        static constexpr auto pos_format = Format::eR32G32B32Sfloat;
        static constexpr auto color_format = Format::eR32G32B32Sfloat;
        static constexpr auto texture_coordinate_format = Format::eR32G32Sfloat;

        constant::format_t<pos_format> pos;
        constant::format_t<color_format> color;
        constant::format_t<texture_coordinate_format> texture_coordinate;
    };

    struct vertex : vertex_base
    {
        using base = vertex_base;
        using base::base;

        constexpr vertex(vec3, vec3, vec2) noexcept;

        static constexpr VertexInputBindingDescription description{
            0,
            sizeof(vertex_base),
            VertexInputRate::eVertex
        };

        static constexpr array<VertexInputAttributeDescription, 3> attribute_descriptions{
            VertexInputAttributeDescription{
            0,
            0,
            pos_format,
            static_cast<uint32_t>(MEMBER_OFFSET(base, pos))
        },
            VertexInputAttributeDescription{
            1,
            0,
            color_format,
            static_cast<uint32_t>(MEMBER_OFFSET(base, color))
        },
            VertexInputAttributeDescription{
            2,
            0,
            texture_coordinate_format,
            static_cast<uint32_t>(MEMBER_OFFSET(base, texture_coordinate))
        }
        };
    };
}

#include "object.tpp"
