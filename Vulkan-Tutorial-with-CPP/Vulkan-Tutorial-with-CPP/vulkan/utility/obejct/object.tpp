#pragma once
#include "object_traits.h"

namespace vulkan::utility
{
    template<typename HandleType>
    object<HandleType>::object(
        info_type info,
        const owner_type& owner,
        const dispatch_type& dispatch,
        const optional<AllocationCallbacks>& allocator
    ) : base(std::move(info))
    {
        initialize(owner, dispatch, allocator);
    }

    template<typename HandleType>
    object<HandleType>::object(
        info_type info,
        const object<owner_type>& owner_object,
        const optional<AllocationCallbacks>& allocator
    ) : base(std::move(info))
    {
        initialize(owner_object, allocator);
    }

    template<typename HandleType>
    void object<HandleType>::initialize(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const optional<AllocationCallbacks>& allocator
    )
    {
        auto info = this->info();
        this->base::base::operator=(create_unique_handle(owner, dispatch, info, allocator));
    }

    template<typename HandleType>
    void object<HandleType>::initialize(
        const object<owner_type>& owner_object,
        const optional<AllocationCallbacks>& allocator
    )
    {
        initialize(*owner_object, owner_object.dispatch(), allocator);
    }

    template<typename T>
    object<PhysicalDevice>::object(const Instance& instance, const T& t)
    {
        for(const auto& device : instance.enumeratePhysicalDevices(DispatchLoaderStatic{})) if(t(device)) physical_device_ = device;
    }

    template<typename HandleType>
    auto pool_object<HandleType>::create_element_objects(
        const owner_type& owner,
        const dispatch_type& dispatch,
        const typename info<element_handle_t>::base_info_type& info
    ) -> vector<object<element_handle_t>>
    {
        return ::utility::container_transform<vector<object<element_handle_t>>>(
            create_element_unique_handles(owner, dispatch, info),
            [&info](typename object_traits<element_handle_t>::base& unique_handle)
            {
                return object<element_handle_t>{info, std::move(unique_handle)};
            }
        );
    }

    template<typename HandleType>
    auto pool_object<HandleType>::create_element_objects(
        const object<owner_type>& owner_object,
        const typename info<element_handle_t>::base_info_type& info
    ) -> vector<object<element_handle_t>>
    {
        return create_element_objects(*owner_object, owner_object.dispatch(), info);
    }

    template<typename HandleType>
    template<typename PoolObjectRangeType>
    void pool_object<HandleType>::element_objects_reset(const object<owner_type>& owner_object, PoolObjectRangeType& range)
    {
        using range_element_type = typename PoolObjectRangeType::value_type;

        if constexpr(std::is_same_v<element_handle_t, range_element_type>) owner_object->free(
            range.front().getPool(),
            range,
            owner_object.dispatch()
        );
        else if constexpr(std::is_same_v<object<element_handle_t>, range_element_type>)
        {
            owner_object->free(range.front().getPool(), ::utility::container_transform<vector<element_handle_t>>(
                range,
                [](const auto& object) { return *object; }
            ), owner_object.dispatch());
            for(auto& object : range) *object = nullptr;
        }
        else static_assert(true, "target type is incompatible");
    }
}