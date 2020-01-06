#pragma once
#include "vulkan/utility/utility_core.h"

namespace vulkan::utility::constant
{
    using namespace vk;

    template<typename T = DeviceSize>
    inline constexpr auto whole_size = T{VK_WHOLE_SIZE};

    template<typename T = uint32_t>
    inline constexpr auto queue_family_ignore = T{VK_QUEUE_FAMILY_IGNORED};

    template<typename T = uint32_t>
    inline constexpr auto subpass_external = T{VK_SUBPASS_EXTERNAL};

    template<Format FormatValue>
    struct format;

    template<>
    struct format<Format::eR8Uint> :value_identity<Format::eR8Uint>, type_identity<uint8_t> {};

    template<>
    struct format<Format::eR8Sint> : value_identity<Format::eR8Sint>, type_identity<int8_t> {};

    template<>
    struct format<Format::eR8Unorm> :value_identity<Format::eR8Unorm>, type_identity<uint8_t> {};

    template<>
    struct format<Format::eR8Snorm> : value_identity<Format::eR8Snorm>, type_identity<int8_t> {};

    template<>
    struct format<Format::eR16Uint> :value_identity<Format::eR16Uint>, type_identity<uint16_t> {};

    template<>
    struct format<Format::eR16Sint> :value_identity<Format::eR16Sint>, type_identity<int16_t> {};

    template<>
    struct format<Format::eR16Sfloat> :value_identity<Format::eR16Sfloat>, type_identity<detail::hdata> {};

    template<>
    struct format<Format::eR16Unorm> :value_identity<Format::eR16Unorm>, type_identity<uint16_t> {};

    template<>
    struct format<Format::eR16Snorm> :value_identity<Format::eR16Snorm>, type_identity<int16_t> {};

    template<>
    struct format<Format::eR32Uint> :value_identity<Format::eR32Uint>, type_identity<uint32_t> {};

    template<>
    struct format<Format::eR32Sint> :value_identity<Format::eR32Sint>, type_identity<int32_t> {};

    template<>
    struct format<Format::eR32Sfloat> :value_identity<Format::eR32Sfloat>, type_identity<float32> {};

    template<>
    struct format<Format::eR64Uint> :value_identity<Format::eR64Uint>, type_identity<uint64_t> {};

    template<>
    struct format<Format::eR64Sint> :value_identity<Format::eR64Sint>, type_identity<int64_t> {};

    template<>
    struct format<Format::eR64Sfloat> :value_identity<Format::eR64Sfloat>, type_identity<float64> {};

    template<>
    struct format<Format::eR8G8Uint> :value_identity<Format::eR8G8Uint>, type_identity<u8vec2> {};

    template<>
    struct format<Format::eR8G8Sint> :value_identity<Format::eR8G8Sint>, type_identity<i8vec2> {};

    template<>
    struct format<Format::eR8G8B8Uint> :value_identity<Format::eR8G8B8Uint>, type_identity<u8vec3> {};

    template<>
    struct format<Format::eR8G8B8Sint> :value_identity<Format::eR8G8B8Sint>, type_identity<i8vec3> {};

    template<>
    struct format<Format::eR8G8B8Unorm> :value_identity<Format::eR8G8B8Unorm>, type_identity<u8vec3> {};

    template<>
    struct format<Format::eR8G8B8Snorm> : value_identity<Format::eR8G8B8Snorm>, type_identity<i8vec3> {};

    template<>
    struct format<Format::eB8G8R8Uint> : value_identity<Format::eB8G8R8Uint>, type_identity<u8vec3> {};

    template<>
    struct format<Format::eB8G8R8Sint> : value_identity<Format::eB8G8R8Sint>, type_identity<i8vec3> {};

    template<>
    struct format<Format::eB8G8R8Unorm> :value_identity<Format::eB8G8R8Unorm>, type_identity<u8vec3> {};

    template<>
    struct format<Format::eB8G8R8Snorm> :value_identity<Format::eB8G8R8Snorm>, type_identity<i8vec3> {};

    template<>
    struct format<Format::eR8G8B8A8Uint> :value_identity<Format::eR8G8B8A8Uint>, type_identity<u8vec4> {};

    template<>
    struct format<Format::eR8G8B8A8Sint> :value_identity<Format::eR8G8B8A8Sint>, type_identity<i8vec4> {};

    template<>
    struct format<Format::eR8G8B8A8Unorm> :value_identity<Format::eR8G8B8A8Unorm>, type_identity<u8vec4> {};

    template<>
    struct format<Format::eR8G8B8A8Snorm> : value_identity<Format::eR8G8B8A8Snorm>, type_identity<i8vec4> {};

    template<>
    struct format<Format::eB8G8R8A8Uint> : value_identity<Format::eB8G8R8A8Uint>, type_identity<u8vec4> {};

    template<>
    struct format<Format::eB8G8R8A8Sint> : value_identity<Format::eB8G8R8A8Sint>, type_identity<i8vec4> {};

    template<>
    struct format<Format::eB8G8R8A8Unorm> :value_identity<Format::eB8G8R8A8Unorm>, type_identity<u8vec4> {};

    template<>
    struct format<Format::eB8G8R8A8Snorm> :value_identity<Format::eB8G8R8A8Snorm>, type_identity<i8vec4> {};

    template<>
    struct format<Format::eR16G16Uint> :value_identity<Format::eR16G16Uint>, type_identity<u16vec2> {};

    template<>
    struct format<Format::eR16G16Sint> :value_identity<Format::eR16G16Sint>, type_identity<i16vec2> {};

    template<>
    struct format<Format::eR16G16Sfloat> :value_identity<Format::eR16G16Sfloat>, type_identity<vec<2,detail::hdata>> {};

    template<>
    struct format<Format::eR16G16Unorm> :value_identity<Format::eR16G16Unorm>, type_identity<u16vec2> {};

    template<>
    struct format<Format::eR16G16Snorm> :value_identity<Format::eR16G16Snorm>, type_identity<i16vec2> {};

    template<>
    struct format<Format::eR16G16B16Uint> :value_identity<Format::eR16G16B16Uint>, type_identity<u16vec3> {};

    template<>
    struct format<Format::eR16G16B16Sint> :value_identity<Format::eR16G16B16Sint>, type_identity<i16vec3> {};

    template<>
    struct format<Format::eR16G16B16Sfloat> :value_identity<Format::eR16G16B16Sfloat>, type_identity<vec<3,detail::hdata>> {};

    template<>
    struct format<Format::eR16G16B16Unorm> :value_identity<Format::eR16G16B16Unorm>, type_identity<u16vec3> {};

    template<>
    struct format<Format::eR16G16B16Snorm> :value_identity<Format::eR16G16B16Snorm>, type_identity<i16vec3> {};

    template<>
    struct format<Format::eR16G16B16A16Uint> :value_identity<Format::eR16G16B16A16Uint>, type_identity<u16vec4> {};

    template<>
    struct format<Format::eR16G16B16A16Sint> :value_identity<Format::eR16G16B16A16Sint>, type_identity<i16vec4> {};

    template<>
    struct format<Format::eR16G16B16A16Sfloat> :value_identity<Format::eR16G16B16A16Sfloat>, type_identity<vec<4,detail::hdata>> {};

    template<>
    struct format<Format::eR16G16B16A16Unorm> :value_identity<Format::eR16G16B16A16Unorm>, type_identity<u16vec4> {};

    template<>
    struct format<Format::eR16G16B16A16Snorm> :value_identity<Format::eR16G16B16A16Snorm>, type_identity<i16vec4> {};

    template<>
    struct format<Format::eR32G32Uint> :value_identity<Format::eR32G32Uint>, type_identity<u32vec2> {};

    template<>
    struct format<Format::eR32G32Sint> :value_identity<Format::eR32G32Sint>, type_identity<i32vec2> {};

    template<>
    struct format<Format::eR32G32Sfloat> :value_identity<Format::eR32G32Sint>, type_identity<f32vec2> {};

    template<>
    struct format<Format::eR32G32B32Uint> :value_identity<Format::eR32G32B32Uint>, type_identity<u32vec3> {};

    template<>
    struct format<Format::eR32G32B32Sint> :value_identity<Format::eR32G32B32Sint>, type_identity<i32vec3> {};

    template<>
    struct format<Format::eR32G32B32Sfloat> :value_identity<Format::eR32G32B32Sfloat>, type_identity<f32vec3> {};

    template<>
    struct format<Format::eR32G32B32A32Uint> :value_identity<Format::eR32G32B32A32Uint>, type_identity<u32vec4> {};

    template<>
    struct format<Format::eR32G32B32A32Sint> :value_identity<Format::eR32G32B32A32Sint>, type_identity<i32vec4> {};

    template<>
    struct format<Format::eR32G32B32A32Sfloat> :value_identity<Format::eR32G32B32A32Sfloat>, type_identity<f32vec4> {};

    template<>
    struct format<Format::eR64G64Uint> :value_identity<Format::eR64G64Uint>, type_identity<u64vec2> {};

    template<>
    struct format<Format::eR64G64Sint> :value_identity<Format::eR64G64Sint>, type_identity<i64vec2> {};

    template<>
    struct format<Format::eR64G64Sfloat> :value_identity<Format::eR64G64Sint>, type_identity<f64vec2> {};

    template<>
    struct format<Format::eR64G64B64Uint> :value_identity<Format::eR64G64B64Uint>, type_identity<u64vec3> {};

    template<>
    struct format<Format::eR64G64B64Sint> :value_identity<Format::eR64G64B64Sint>, type_identity<i64vec3> {};

    template<>
    struct format<Format::eR64G64B64Sfloat> :value_identity<Format::eR64G64B64Sfloat>, type_identity<f64vec3> {};

    template<>
    struct format<Format::eR64G64B64A64Uint> :value_identity<Format::eR64G64B64A64Uint>, type_identity<u64vec4> {};

    template<>
    struct format<Format::eR64G64B64A64Sint> :value_identity<Format::eR64G64B64A64Sint>, type_identity<i64vec4> {};

    template<>
    struct format<Format::eR64G64B64A64Sfloat> :value_identity<Format::eR64G64B64A64Sfloat>, type_identity<f64vec4> {};

    template<Format FormatValue>
    inline constexpr auto format_v = format<FormatValue>::value;

    template<Format FormatValue>
    using format_t = typename format<FormatValue>::type;

    template<typename T>
    inline constexpr auto index_type = IndexType::eNoneNV;

    template<>
    inline constexpr auto index_type<uint32_t> = IndexType::eUint32;

    template<>
    inline constexpr auto index_type<uint16_t> = IndexType::eUint16;

    template<>
    inline constexpr auto index_type<uint8_t> = IndexType::eUint8EXT;

    constexpr pair<AccessFlags, PipelineStageFlagBits> required_access_and_pipeline_stage(ImageLayout);
}

#include "constant.tpp"
