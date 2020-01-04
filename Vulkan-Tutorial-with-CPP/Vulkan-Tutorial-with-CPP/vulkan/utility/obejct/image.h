#pragma once
#include "vulkan/utility/obejct/object.h"
#include "static_memory.h"

namespace vulkan::utility
{
    constexpr auto to_image_view_type(const ImageType image_type);

    [[nodiscard]] pair<device_memory_object, vector<DeviceSize>> generate_image_memory_info(
        const device_object&,
        const vector<Image>&,
        const PhysicalDevice&,
        const MemoryPropertyFlags);

    void write_transfer_image_layout_command(
        const CommandBuffer,
        const Image,
        const ImageSubresourceRange,
        const ImageLayout,
        const ImageLayout,
        const DispatchLoaderDynamic&
    );

    template<Format FormatValue>
    class texture_image
    {
    public:
        static constexpr auto format_value = FormatValue;
    private:
        buffer_object buffer_;
        device_memory_object buffer_memory_;

        image_object image_;
        device_memory_object image_memory_;

        image_view_object image_view_;

        static constexpr auto max_anisotropy = 16;

    public:
        constexpr texture_image() noexcept = default;

        constexpr texture_image(
            const ImageType,
            const Extent3D,
            const optional<ImageCreateFlags>  = {},
            const optional<ComponentMapping>  = {},
            const optional<
                pair<decltype(ImageCreateInfo::mipLevels), decltype(ImageCreateInfo::arrayLayers)>
            >& = {},
            const optional<SampleCountFlagBits>  = {}
        ) noexcept;

        void initialize(const device_object& device_object, const PhysicalDevice physical_device);

        void write_from_src(
            const device_object&,
            const constant::format_t<format_value>&,
            const constant::format_t<format_value>&
        ) const;

        void write_transfer_command(const device_object& device_object, const CommandBuffer&) const;


        void write_blit_command(const device_object& device_object, const CommandBuffer&) const;

        constexpr const auto& buffer() const;

        constexpr const auto& buffer_memory() const;

        constexpr const auto& image() const;

        constexpr const auto& image_memory() const;

        constexpr const auto& image_view() const;
    };

    class depth_image
    {
        image_object image_;
        device_memory_object image_memory_;

        image_view_object image_view_;

        static constexpr auto max_anisotropy = 16;

    public:
        constexpr depth_image() noexcept = default;

        constexpr depth_image(
            const Format,
            const ImageType,
            const Extent3D) noexcept;

        void initialize(const device_object&, const PhysicalDevice);

        constexpr const auto& image() const;

        constexpr const auto& image_memory() const;

        constexpr const auto& image_view() const;
    };
}

#include "image.tpp"
