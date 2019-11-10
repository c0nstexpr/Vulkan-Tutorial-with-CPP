#pragma once

namespace vk
{
    template<typename Dispatch>
    UniqueHandle<GraphicsPipeline, Dispatch>::UniqueHandle(base&& base_handle) :
        base(std::move(base_handle))
    {}
}

namespace vulkan::utility
{
    template<typename HandleType, typename DispatchType>
    object_traits<HandleType, DispatchType>::object_traits(std::nullptr_t) {}

    template<typename HandleType, typename DispatchType>
    object_traits<HandleType, DispatchType>::object_traits(info_type info, base base_handle) :
        base(std::move(base_handle)),
        info_(std::move(info))
    {}

    template<typename HandleType, typename DispatchType>
    const auto& object_traits<HandleType, DispatchType>::info() const { return info_; }
}