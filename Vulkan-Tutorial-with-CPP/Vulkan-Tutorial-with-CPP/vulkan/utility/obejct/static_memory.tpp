#pragma once

namespace vulkan::utility
{
    template<typename Input>
    void write(
        const device_memory_object& memory_object,
        const device_object& device,
        const Input data_begin,
        const Input data_end,
        const DeviceSize offset
    )
    {
        using data_element_type = std::decay_t<decltype(*data_begin)>;

        const size_t data_size = sizeof(data_element_type) * std::distance(data_begin, data_end);

        if(offset + data_size > static_cast<MemoryAllocateInfo>(memory_object.info()).allocationSize)
            throw std::
            runtime_error("data size is out of destination memory range");

        std::copy(
            data_begin,
            data_end,
            static_cast<data_element_type*>(device->mapMemory(
                *memory_object,
                offset,
                data_size,
                {},
                device.dispatch()
            ))
        );
        device->unmapMemory(*memory_object, device.dispatch());
    }

    template<typename T>
    void write(
        const device_memory_object& memory_object,
        const device_object& device,
        const T& data,
        const DeviceSize offset
    ) { write(memory_object, device, &data, &data + 1, offset); }

    template<bool Cached, typename ... Types>
    template<template<typename T> typename RangeType>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::generate_buffer_info(
        const array<BufferUsageFlags, type_list::size>& usages
    )
    {
        //ReSharper disable CppEntityAssignedButNoRead
        //for_each change the element content
        ::utility::for_each(
            [&usages](
                decltype(*device_local_buffers_.begin()) local_buffer,

                // ReSharper disable CppEntityAssignedButNoRead
                decltype(*host_buffers_.begin()) host_buffer,
                decltype(*usages.cbegin()) usage,
                decltype(*type_sizes.cbegin()) type_size,
                decltype(*sizes_.cbegin()) size
            )
            {
                const size_t memory_size = type_size * size;
                local_buffer = buffer_object{{{{}, memory_size, usage | BufferUsageFlagBits::eTransferDst}}};
                host_buffer = buffer_object{{{{}, memory_size, BufferUsageFlagBits::eTransferSrc}}};
            },
            device_local_buffers_.begin(),
            device_local_buffers_.end(),
            host_buffers_.begin(),
            usages.cbegin(),
            type_sizes.cbegin(),
            sizes_.cbegin()
        );
        // ReSharper restore CppEntityAssignedButNoRead
    }

    template<bool Cached, typename ... Types>
    template<template<typename T> typename RangeType>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::generate_memory_info(
        const PhysicalDevice& physical_device
    )

    {
        {
            auto&& [memory, type_offsets] = generate_buffer_memory_info(
                *device_,
                ::utility::container_transform<vector<Buffer>>(
                    host_buffers_,
                    [](const auto& buffer_object) { return *buffer_object; }
                ),
                physical_device,
                host_memory_property
            );
            host_memory_ = std::move(memory);
            type_offsets_ = ::utility::container_transform<decltype(type_offsets_)>(type_offsets);
        }

        device_local_memory_ = generate_buffer_memory_info(
            *device_,
            ::utility::container_transform<vector<Buffer>>(
                device_local_buffers_,
                [](const auto& buffer_object) { return *buffer_object; }
            ),
            physical_device,
            device_memory_property
        ).first;
    }

    template<bool Cached, typename ... Types>
    template<template<typename T> typename RangeType>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::bind_buffer_memory() const
    {
        ::utility::for_each(
            [this](
                decltype(*device_local_buffers_.cbegin()) local_buffer,
                decltype(*host_buffers_.cbegin()) host_buffer,
                decltype(*type_offsets_.cbegin()) offset
            )
            {
                (*device_)->bindBufferMemory(
                    *local_buffer,
                    *device_local_memory_,
                    offset,
                    device_->dispatch()
                );
                (*device_)->bindBufferMemory(*host_buffer, *host_memory_, offset, device_->dispatch());
            },
            device_local_buffers_.cbegin(),
            device_local_buffers_.cend(),
            host_buffers_.cbegin(),
            type_offsets_.cbegin()
        );
    }

    template<bool Cached, typename... Types>
    template<template<typename T> typename RangeType>
    template<typename T, typename Op>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::write_impl(
        value_type<T> value,
        [[maybe_unused]] const Op& op
    )
    {
        if(value.size() != type_sizes[type_index<T>]) throw std::out_of_range{"input value out of range"};
        if(host_memory_)
            if constexpr(std::is_same_v<Op, empty_type>)
                utility::write(
                    host_memory_,
                    *device_,
                    value.cbegin(),
                    value.cend(),
                    type_offsets_[type_index<T>]
                );
            else
                for(const auto& element : value)
                {
                    const auto& [cbegin, cend] = op(element);
                    utility::write(host_memory_, *device_, cbegin, cend, type_offsets_[type_index<T>]);
                }
        std::get<decltype(value)>(type_values_) = std::move(value);
    }

    template<bool Cached, typename ... Types>
    template<template<typename T> typename RangeType>
    static_memory<Cached, Types...>::base_array_values<RangeType>::base_array_values(
        const device_object& device,
        const array<BufferUsageFlags, type_list::size>& usages,
        const decltype(sizes_)& sizes
    ) : device_(&device), sizes_(sizes) { generate_buffer_info(usages); }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    static_memory<Cached, Types...>::base_array_values<RangeType>::base_array_values(
        const PhysicalDevice& physical_device,
        const device_object& device,
        const array<BufferUsageFlags, type_list::size>& usages,
        const decltype(sizes_)& size
    ) : base_array_values(device, usages, size) { initialize(physical_device); }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::initialize(
        const PhysicalDevice& physical_device
    )
    {
        ::utility::for_each(
            [this](
                decltype(*device_local_buffers_.begin()) local_buffer,
                decltype(*host_buffers_.begin()) host_buffer
            )
            {
                local_buffer.initialize(*device_);
                host_buffer.initialize(*device_);
            },
            device_local_buffers_.begin(),
            device_local_buffers_.end(),
            host_buffers_.begin()
        );

        generate_memory_info(physical_device);

        device_local_memory_.initialize(*device_);
        host_memory_.initialize(*device_);

        bind_buffer_memory();
    }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    template<typename ... T>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::write(value_type<T> ... values)
    {
        (write_impl<T>(std::move(values), empty_type{}), ...);
    }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    template<typename T, typename Op>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::write(value_type<T> value, const Op& op)
    {
        write_impl<T>(value, op);
    }

    template<bool Cached, typename... Types>
    template<template <typename T> class RangeType>
    template<typename... T>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::flush()
    {
        ((*device_)->flushMappedMemoryRanges(
            {
            MappedMemoryRange{
            *host_memory_,
            type_offsets_[type_index<T>],
            constant::whole_size<DeviceSize>
            }
            },
            device_->dispatch()
        ), ...);
    }

    template<bool Cached, typename... Types>
    template<template <typename T> class RangeType>
    template<typename T>
    constexpr const auto& static_memory<Cached, Types...>::base_array_values<RangeType>::read() const
    {
        return std::get<type_index<T>>(type_values_);
    }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    void static_memory<Cached, Types...>::base_array_values<RangeType>::write_transfer_command(
        const CommandBuffer& command_buffer
    ) const
    {
        ::utility::for_each(
            [dispatch = device_->dispatch(), &command_buffer](
                decltype(*device_local_buffers_.cbegin()) device_local_buffer,
                decltype(*host_buffers_.cbegin()) host_buffer
            )
            {
                command_buffer.copyBuffer(
                    *host_buffer,
                    *device_local_buffer,
                    {{0, 0, host_buffer.info().info.size}},
                    dispatch
                );
            },
            device_local_buffers_.cbegin(),
            device_local_buffers_.cend(),
            host_buffers_.cbegin()
        );
    }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    constexpr const auto& static_memory<Cached, Types...>::base_array_values<RangeType>::device_local_buffer(
        const size_t i
    ) const { return device_local_buffers_[i]; }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    constexpr const auto& static_memory<Cached, Types...>::base_array_values<RangeType>::host_buffer(
        const size_t i
    ) const { return host_buffers_[i]; }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    constexpr const auto& static_memory<Cached, Types...>::base_array_values<RangeType>::device_local_memory() const
    {
        return device_local_memory_;
    }

    template<bool Cached, typename ... Types>
    template<template <typename T> class RangeType>
    constexpr const auto& static_memory<Cached, Types...>::base_array_values<RangeType>::host_memory() const
    {
        return host_memory_;
    }

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    static_memory<Cached, Types...>::array_values<Counts...>::array_values(
        const device_object& device,
        const array<BufferUsageFlags, type_list::size>& usages
    ) : base(device, usages, traits::counts) { }

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    static_memory<Cached, Types...>::array_values<Counts...>::array_values(
        const PhysicalDevice& physical_device,
        const device_object& device,
        const array<BufferUsageFlags, type_list::size>& usages
    ) : base(physical_device, device, usages, traits::counts) { }
}
