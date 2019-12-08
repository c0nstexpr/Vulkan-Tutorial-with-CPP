#pragma once

#include "static_memory.h"

namespace vulkan::utility
{
    optional<decltype(MemoryAllocateInfo::memoryTypeIndex)> search_memory_type_index(
        const PhysicalDevice& physical_device,
        const DispatchLoaderDynamic& dispatch,
        const MemoryPropertyFlags property_flags,
        const decltype(MemoryRequirements::memoryTypeBits) require_memory_type_bits
    )
    {
        const bitset<sizeof require_memory_type_bits * ::utility::constant::char_bit> require_memory_type_bitset{require_memory_type_bits};
        vector<MemoryType> memory_types;
        {
            const PhysicalDeviceMemoryProperties& properties = physical_device.getMemoryProperties(dispatch);

            memory_types = {
                properties.memoryTypes,
                properties.memoryTypes + properties.memoryTypeCount
            };
        }
        for(
            decltype(MemoryAllocateInfo::memoryTypeIndex) index = 0;
            index < require_memory_type_bitset.size();
            ++index
            )
            if(require_memory_type_bitset[index] &&
                (memory_types[index].propertyFlags & property_flags) == property_flags)
                return index;
        return nullopt;
    }

    pair<DeviceSize, vector<DeviceSize>> generate_memory_size_and_offsets(vector<pair<MemoryRequirements, size_t>>& requirements)
    {
        DeviceSize memory_size = 0;
        vector<DeviceSize> offsets(requirements.size());
        std::sort(
            requirements.begin(),
            requirements.end(),
            [](decltype(*requirements.begin()) left, decltype(*requirements.begin()) right)
            {
                return left.first.size < right.first.size;
            }
        );

        for(const auto& requirement : requirements)
        {
            memory_size += memory_size % requirement.first.alignment;//allgin
            offsets[requirement.second] = memory_size;
            memory_size += requirement.first.size;
        }
        return {memory_size, offsets};
    }

    pair<device_memory_object, vector<DeviceSize>> generate_buffer_memory_info(
        const device_object& device,
        const vector<Buffer>& buffers,
        const PhysicalDevice& physical_device,
        const MemoryPropertyFlags property_flags
    )
    {
        vector<pair<MemoryRequirements, size_t>> requirements(buffers.size());
        decltype(MemoryAllocateInfo::memoryTypeIndex) memory_index;

        std::transform(
            buffers.cbegin(),
            buffers.cend(),
            requirements.begin(),
            [i = size_t{0}, &device](decltype(*buffers.cbegin()) buffer) mutable -> decltype(requirements)::value_type
            {
                return {device->getBufferMemoryRequirements(buffer, device.dispatch()), i++};
            }
        );

        {
            const auto& index = search_memory_type_index(
                physical_device,
                device.dispatch(),
                property_flags,
                std::accumulate(requirements.cbegin(), requirements.cend(), 0, [](
                    const decltype(MemoryRequirements::memoryTypeBits) type,
                    decltype(*requirements.cbegin()) requirement
                    )
                    {
                        return type | requirement.first.memoryTypeBits;
                    }
                )
            );
            if(!index)
                throw std::runtime_error{"unable to fetch suitable memory type"};

            memory_index = *index;
        }

        auto&& [size, offsets] = generate_memory_size_and_offsets(requirements);

        return {device_memory_object{{size, memory_index}}, std::move(offsets)};
    }
}