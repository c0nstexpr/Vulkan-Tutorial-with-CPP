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
        object(std::nullptr_t = {});

        template<typename T>
        object(const Instance&, const T&);

        handle_type& operator*();
        const handle_type& operator*() const;
        handle_type* operator->();
        const handle_type* operator->() const;
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

        const DispatchLoaderDynamic& dispatch() const;
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

        const DispatchLoaderDynamic& dispatch() const;
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

    using instance = object<Instance>;
    using device = object<Device>;
    using physical_device = object<PhysicalDevice>;
    using debug_messenger = object<DebugUtilsMessengerEXT>;
    using surface = object<SurfaceKHR>;
    using swap_chain = object<SwapchainKHR>;
    using image_view = object<ImageView>;
    using render_pass = object<RenderPass>;
    using frame_buffer = object<Framebuffer>;
    using shader_module = object<ShaderModule>;
    using buffer = object<Buffer>;
    using image = object<Image>;
    using descriptor_set_layout = object<DescriptorSetLayout>;
    using descriptor_pool = pool_object<DescriptorPool>;
    using descriptor_set = object<DescriptorSet>;
    using pipeline_layout = object<PipelineLayout>;
    using device_memory = object<DeviceMemory>;
    using graphics_pipeline = object<GraphicsPipeline>;
    using command_pool = pool_object<CommandPool>;
    using command_buffer = object<CommandBuffer>;
    using semaphore = object<Semaphore>;
    using fence = object<Fence>;

    struct vertex
    {
        vec2 pos;
        vec3 color;

        static const VertexInputBindingDescription description;
        static const array<VertexInputAttributeDescription, 2> attribute_descriptions;
    };
}

#include "object.tpp"
