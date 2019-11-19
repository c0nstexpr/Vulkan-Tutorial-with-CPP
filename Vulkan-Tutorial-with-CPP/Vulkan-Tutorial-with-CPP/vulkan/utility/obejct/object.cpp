#include "object.h"

namespace vulkan::utility
{
    template<>
    auto object<DebugUtilsMessengerEXT>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createDebugUtilsMessengerEXTUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<SurfaceKHR>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        VkSurfaceKHR surface_khr = nullptr;
        if(allocator)
        {
            const VkAllocationCallbacks& vk_allocator = *allocator;
            glfwCreateWindowSurface(owner, info.window, &vk_allocator, &surface_khr);
        }
        else glfwCreateWindowSurface(owner, info.window, nullptr, &surface_khr);

        return base::base{surface_khr, {
            owner,
            allocator ? *allocator : null_opt<const AllocationCallbacks>,
            dispatch
        }};
    }

    template<>
    auto object<SwapchainKHR>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createSwapchainKHRUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<ImageView>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createImageViewUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<RenderPass>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createRenderPassUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<ShaderModule>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createShaderModuleUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<DescriptorSetLayout>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createDescriptorSetLayoutUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<DescriptorPool>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createDescriptorPoolUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<PipelineLayout>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createPipelineLayoutUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<Framebuffer>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createFramebufferUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<CommandPool>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createCommandPoolUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<Semaphore>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createSemaphoreUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<Fence>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createFenceUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<Buffer>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createBufferUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<DeviceMemory>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.allocateMemoryUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    template<>
    auto object<Image>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return owner.createImageUnique(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch);
    }

    auto object<GraphicsPipeline>::create_unique_handle(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return base::base{owner.createGraphicsPipelineUnique(info.cache, info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>, dispatch)};
    }

    template<>
    auto object<GraphicsPipeline>::create_unique_handles(
        const owner_type& owner,
        const dispatch_type& dispatch,
        vector<base_info_type>& infos,
        const optional<AllocationCallbacks>& allocator
    ) -> vector<base::base>
    {
        if(infos.size() == 0)return {};

        return ::utility::container_transform<vector<base::base>>(owner.createGraphicsPipelinesUnique(
            infos.front().cache,
            vector<GraphicsPipelineCreateInfo>{infos.cbegin(), infos.cend()},
            allocator ? *allocator : null_opt<const AllocationCallbacks>,
            dispatch
        ));
    }

    template<>
    auto pool_object<DescriptorPool>::create_element_unique_handles(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const DescriptorSetAllocateInfo& info
    ) -> vector<UniqueHandle<element_handle_t, DispatchLoaderDynamic>>
    {
        return owner.allocateDescriptorSetsUnique(info, dispatch);
    }

    template<>
    auto pool_object<CommandPool>::create_element_unique_handles(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const CommandBufferAllocateInfo& info
    ) -> vector<UniqueHandle<element_handle_t, DispatchLoaderDynamic>>
    {
        return owner.allocateCommandBuffersUnique(info, dispatch);
    }

    object<PhysicalDevice>::object(std::nullptr_t) {}

    auto object<PhysicalDevice>::operator*() -> handle_type& { return physical_device_; }

    auto object<PhysicalDevice>::operator*() const -> const handle_type& { return physical_device_; }

    auto object<PhysicalDevice>::operator->() -> handle_type* { return &physical_device_; }

    auto object<PhysicalDevice>::operator->() const -> const handle_type* { return &physical_device_; }

    auto object<Instance>::create_unique_handle(
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return createInstanceUnique<DispatchLoaderStatic>(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>);
    }

    void object<Instance>::initialize(const optional<AllocationCallbacks>& allocator)
    {
        this->base::base::operator=(create_unique_handle(info(), allocator));
        dispatch_.init(**this);
    }

    const DispatchLoaderDynamic& object<Instance>::dispatch() const { return dispatch_; }

    auto object<Device>::create_unique_handle(
        const PhysicalDevice& physical_device,
        const base_info_type& info,
        const optional<AllocationCallbacks>& allocator
    ) -> base::base
    {
        return physical_device.createDeviceUnique<DispatchLoaderStatic>(info, allocator ?
            *allocator :
            null_opt<const AllocationCallbacks>);
    }

    void object<Device>::initialize(
        const PhysicalDevice& physical_device,
        const DispatchLoaderDynamic& dispatch,
        const optional<AllocationCallbacks>& allocator
    )
    {
        this->base::base::operator=(create_unique_handle(physical_device, info(), allocator));
        dispatch_ = dispatch;
        dispatch_.init(**this);
    }

    const DispatchLoaderDynamic& object<Device>::dispatch() const { return dispatch_; }
    const VertexInputBindingDescription vertex::description = {0, sizeof(vertex), VertexInputRate::eVertex};
    const array<VertexInputAttributeDescription, 2> vertex::attribute_descriptions = {
        VertexInputAttributeDescription{
        0,
        0,
        constant::format<vec2>,
        static_cast<uint32_t>(MEMBER_OFFSET(vertex, pos))
    },
        VertexInputAttributeDescription{
        1,
        0,
        constant::format<vec3>,
        static_cast<uint32_t>(MEMBER_OFFSET(vertex, color))
    }
    };
}