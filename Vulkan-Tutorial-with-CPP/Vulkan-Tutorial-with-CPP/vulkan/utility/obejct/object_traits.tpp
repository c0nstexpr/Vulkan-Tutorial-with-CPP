#pragma once

namespace vk
{
    template<typename Dispatch>
    UniqueHandle<GraphicsPipeline, Dispatch>::UniqueHandle(base&& base_handle) noexcept :
        base(std::move(base_handle))
    {}
}

namespace vulkan::utility
{
    template<typename HandleType, typename DispatchType>
    constexpr object_traits<HandleType, DispatchType>::object_traits(std::nullptr_t) noexcept {}

    template<typename HandleType, typename DispatchType>
    constexpr object_traits<HandleType, DispatchType>::object_traits(info_type info, base base_handle) noexcept :
        base(std::move(base_handle)),
        info_(std::move(info))
    {}

    template<typename HandleType, typename DispatchType>
    constexpr const auto& object_traits<HandleType, DispatchType>::info() const noexcept { return info_; }
}