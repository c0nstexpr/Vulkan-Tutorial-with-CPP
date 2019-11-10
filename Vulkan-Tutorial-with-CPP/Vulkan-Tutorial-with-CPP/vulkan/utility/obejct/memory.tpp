#pragma once
#include "utility/utility.tpp"

namespace vulkan::utility
{
    template<typename Input>
    void write(
        const device_memory& memory_object,
        const device& device,
        const Input data_begin,
        const Input data_end,
        const DeviceSize offset
    )
    {
        using data_element_type = std::decay_t<decltype(*data_begin)>;

        const size_t data_size = sizeof(data_element_type) * std::distance(data_begin, data_end);

        if(offset + data_size > static_cast<MemoryAllocateInfo>(memory_object.info()).allocationSize)
            throw std::runtime_error("data size is out of destination memory range");

        std::copy(data_begin, data_end, static_cast<data_element_type*>(device->mapMemory(
            *memory_object,
            offset,
            data_size,
            {},
            device.dispatch()
        )));
        device->unmapMemory(*memory_object, device.dispatch());
    }

    template<typename T>
    void write(const device_memory& memory_object, const device& device, const T& data, const DeviceSize offset)
    {
        write(memory_object, device, &data, &data + 1, offset);
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    void memory<Cached, Types...>::array_values<Counts...>::generate_buffer_info(
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
                decltype(*counts.cbegin()) count
                )
            {
                const size_t memory_size = type_size * count;
                local_buffer = buffer{{{{}, memory_size, usage | BufferUsageFlagBits::eTransferDst}}};
                host_buffer = buffer{{{{}, memory_size, BufferUsageFlagBits::eTransferSrc}}};
            },
            device_local_buffers_.begin(),
                device_local_buffers_.end(),
                host_buffers_.begin(),
                usages.cbegin(),
                type_sizes.cbegin(),
                counts.cbegin()
                );

        // ReSharper restore CppEntityAssignedButNoRead
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    void memory<Cached, Types...>::array_values<Counts...>::generate_memory_info(const PhysicalDevice& physical_device)
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
    template<size_t... Counts>
    void memory<Cached, Types...>::array_values<Counts...>::bind_buffer_memory() const
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
    template<size_t... Counts>
    template<typename T>
    void memory<Cached, Types...>::array_values<Counts...>::write_impl(value_type<T> values)
    {
        if constexpr(std::is_same_v<value_type<T>, T>)
            utility::write(host_memory_, *device_, values, type_offsets_[type_index<T>]);
        else
            utility::write(host_memory_, *device_, values.cbegin(), values.cend(), type_offsets_[type_index<T>]);
        std::get<decltype(values)>(type_values_) = std::move(values);
    }

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    memory<Cached, Types...>::array_values<Counts...>::array_values(const device& device) :
        device_(&device)
    {}

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    memory<Cached, Types...>::array_values<Counts...>::array_values(
        const device& device,
        const array<BufferUsageFlags, type_list::size>& usages
    ) : array_values(device)
    {
        generate_buffer_info(usages);
    }

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    memory<Cached, Types...>::array_values<Counts...>::array_values(
        const device& device,
        const array<BufferUsageFlags, type_list::size>& usages,
        const PhysicalDevice& physical_device
    ) : array_values(device, usages)
    {
        initialize(physical_device);
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    void memory<Cached, Types...>::array_values<Counts...>::initialize(const PhysicalDevice& physical_device)
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

    /* unable to separate the definition and deceleration
    template<bool Cached, typename... Types>
    template<size_t... Counts>
    template<typename... T>
    void memory<Cached, Types...>::array_values<Counts...>::write(
        std::tuple_element_t<type_index<T>, decltype(type_values_)>... values
    )
    {
        (write(host_memory_, device_, values.cbegin(), values.cend(), type_offsets_[type_index<T>]), ...);
        ((std::get<decltype(values)>(type_values_) = std::move(values)), ...);
    }
    */

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    template<typename... T>
    void memory<Cached, Types...>::array_values<Counts...>::flush()
    {
        ((*device_)->flushMappedMemoryRanges({MappedMemoryRange{
            *host_memory_,
            type_offsets_[type_index<T>],
            constant::whole_size<DeviceSize>
            }}, device_->dispatch()), ...);
    }

    template<bool Cached, typename... Types>
    template<size_t... Counts>
    template<typename T>
    constexpr const auto& memory<Cached, Types...>::array_values<Counts...>::read() const
    {
        return std::get<type_index<T>>(type_values_);
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    void memory<Cached, Types...>::array_values<Counts...>::write_transfer_command(const command_buffer& command_buffer) const
    {
        ::utility::for_each([dispatch = device_->dispatch(), &command_buffer](
            decltype(*device_local_buffers_.cbegin()) device_local_buffer,
            decltype(*host_buffers_.cbegin()) host_buffer
            )
        {
            command_buffer->copyBuffer(
                *host_buffer,
                *device_local_buffer,
                {{0, 0, host_buffer.info().info.size}},
                dispatch
            );
        }, device_local_buffers_.cbegin(), device_local_buffers_.cend(), host_buffers_.cbegin());
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    constexpr const auto& memory<Cached, Types...>::array_values<Counts...>::device_local_buffer(const size_t i) const
    {
        return device_local_buffers_[i];
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    constexpr const auto& memory<Cached, Types...>::array_values<Counts...>::host_buffer(const size_t i) const
    {
        return host_buffers_[i];
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    constexpr const auto& memory<Cached, Types...>::array_values<Counts...>::device_local_memory() const
    {
        return device_local_memory_;
    }

    template<bool Cached, typename ... Types>
    template<size_t... Counts>
    constexpr const auto& memory<Cached, Types...>::array_values<Counts...>::host_memory() const
    {
        return host_memory_;
    }
}