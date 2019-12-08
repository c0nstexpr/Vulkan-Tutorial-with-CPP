#pragma once

namespace vulkan::utility
{
    constexpr auto to_image_view_type(const ImageType image_type)
    {
        switch(image_type)
        {
        case ImageType::e1D: return ImageViewType::e1D;
        case ImageType::e2D: return ImageViewType::e2D;
        case ImageType::e3D: return ImageViewType::e3D;
        }
        throw std::invalid_argument{"unknown image type"};
    }

    template<Format FormatValue>
    constexpr texture_image<FormatValue>::texture_image(
        const ImageType image_type,
        const Extent3D extent,
        const optional<ImageCreateFlags> flags,
        const optional<ComponentMapping> component,
        const optional<pair<
        decltype(ImageCreateInfo::mipLevels),
        decltype(ImageCreateInfo::arrayLayers)
        >>&mipmap,
        const optional<SampleCountFlagBits> sampler_count
    ) noexcept :
        buffer_(BufferCreateInfo{
            {},
            extent.width * extent.height * sizeof(constant::format_t<format_value>),
            BufferUsageFlagBits::eTransferSrc
            }),
        image_(image_object::base_info_type{
        flags ? *flags : ImageCreateFlags{},
        image_type,
        format_value,
        extent,
        mipmap ? mipmap->first : 1,
        mipmap ? mipmap->second : 1,
        sampler_count ? *sampler_count : SampleCountFlagBits::e1,
        ImageTiling::eOptimal,
        ImageUsageFlagBits::eTransferDst | ImageUsageFlagBits::eSampled,
            }),
            image_view_(image_view_object::base_info_type{
                {},
                nullptr,
                to_image_view_type(image_type),
                format_value,
                component ? *component : ComponentMapping{},
                {
                    ImageAspectFlagBits::eColor,
                    0,
                    image_.info().info.mipLevels,
                    0,
                    image_.info().info.arrayLayers
                }
                })

    {}

    template<Format FormatValue>
    void texture_image<FormatValue>::initialize(
        const device_object& device_object,
        const PhysicalDevice physical_device
    )
    {
        buffer_.initialize(device_object);
        buffer_memory_ = generate_buffer_memory_info(
            device_object,
            {*buffer_},
            physical_device,
            MemoryPropertyFlagBits::eHostVisible
        ).first;
        buffer_memory_.initialize(device_object);
        device_object->bindBufferMemory(
            *buffer_,
            *buffer_memory_,
            0,
            device_object.dispatch()
        );

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

    template<Format FormatValue>
    void texture_image<FormatValue>::write_from_src(
        const device_object& device_object,
        const constant::format_t<format_value>& begin,
        const constant::format_t<format_value>& end
    ) const
    {
        write(buffer_memory_, device_object, &begin, &end);
    }

    template<Format FormatValue>
    void texture_image<FormatValue>::write_transfer_command(
        const device_object& device_object,
        const CommandBuffer& command_buffer
    ) const
    {
        const ImageSubresourceRange& sub_resource_range = image_view_.info().subresourceRange;

        write_transfer_image_layout_command(
            command_buffer,
            *image_,
            image_view_.info().subresourceRange,
            ImageLayout::eUndefined,
            ImageLayout::eTransferDstOptimal,
            device_object.dispatch()
        );

        command_buffer.copyBufferToImage(
            *buffer_,
            *image_,
            ImageLayout::eTransferDstOptimal,
            {{0, 0, 0, ImageSubresourceLayers{
                sub_resource_range.aspectMask,
                sub_resource_range.baseMipLevel,
                sub_resource_range.baseArrayLayer,
                sub_resource_range.layerCount
            }, {0, 0, 0}, image_.info().info.extent}},
            device_object.dispatch()
        );
    }

    template<Format FormatValue>
    constexpr const auto& texture_image<FormatValue>::buffer() const { return buffer_; }

    template<Format FormatValue>
    constexpr const auto& texture_image<FormatValue>::buffer_memory() const
    {
        return buffer_memory_;
    }

    template<Format FormatValue>
    constexpr const auto& texture_image<FormatValue>::image() const { return image_; }

    template<Format FormatValue>
    constexpr const auto& texture_image<FormatValue>::image_memory() const { return image_memory_; }

    template<Format FormatValue>
    constexpr const auto& texture_image<FormatValue>::image_view() const { return image_view_; }

    //constexpr depth_image::depth_image() noexcept {}

    constexpr depth_image::depth_image(const Format format, const ImageType image_type, const Extent3D extent) noexcept :
        image_(image_object::base_info_type{
            {},
            image_type,
            format,
            extent,
            1,
            1,
            SampleCountFlagBits::e1,
            ImageTiling::eOptimal,
            ImageUsageFlagBits::eDepthStencilAttachment
            }),
        image_view_(image_view_object::base_info_type{
            {},
            nullptr,
            to_image_view_type(image_type),
            format,
            {},
            {ImageAspectFlagBits::eDepth, 0, 1, 0, 1}
            })
    {}

    constexpr const auto& depth_image::image() const { return image_; }

    constexpr const auto& depth_image::image_memory() const { return image_memory_; }

    constexpr const auto& depth_image::image_view() const { return image_view_; }
}