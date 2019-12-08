#include "image.h"

namespace vulkan::utility
{
    pair<device_memory_object, vector<DeviceSize>> generate_image_memory_info(
        const device_object& device,
        const vector<Image>& images,
        const PhysicalDevice& physical_device,
        const MemoryPropertyFlags property_flags
    )
    {
        vector<pair<MemoryRequirements, size_t>> requirements(images.size());
        decltype(MemoryAllocateInfo::memoryTypeIndex) memory_index;

        std::transform(
            images.cbegin(),
            images.cend(),
            requirements.begin(),
            [i = size_t{0}, &device](decltype(*images.cbegin()) image) mutable -> decltype(requirements)::value_type
            {
                return {device->getImageMemoryRequirements(image, device.dispatch()), i++};
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

    void write_transfer_image_layout_command(
        const CommandBuffer command_buffer,
        const Image image,
        const ImageSubresourceRange sub_resource_range,
        const ImageLayout src_layout,
        const ImageLayout dst_layout,
        const DispatchLoaderDynamic& dispatch
    )
    {
        auto&& [src_access, src_stage] = constant::required_access_and_pipeline_stage(src_layout);
        auto&& [dst_access, dst_stage] = constant::required_access_and_pipeline_stage(dst_layout);

        command_buffer.pipelineBarrier(src_stage, dst_stage, {}, {}, {}, {{
                src_access,
                dst_access,
                src_layout,
                dst_layout,
                constant::queue_family_ignore<>,
                constant::queue_family_ignore<>,
                image,
                sub_resource_range,
            }}, dispatch);
    }

    void depth_image::initialize(const device_object& device_object, const PhysicalDevice physical_device)
    {
        image_.initialize(device_object);
        image_memory_ = generate_image_memory_info(
            device_object,
            {*image_},
            physical_device,
            MemoryPropertyFlagBits::eDeviceLocal
        ).first;
        image_memory_.initialize(device_object);
        device_object->bindImageMemory(*image_, *image_memory_, 0, device_object.dispatch());

        {
            image_view_object::base_info_type info = image_view_.info();
            info.image = *image_;
            image_view_ = image_view_object{info};
        }
        image_view_.initialize(device_object);
    }
}