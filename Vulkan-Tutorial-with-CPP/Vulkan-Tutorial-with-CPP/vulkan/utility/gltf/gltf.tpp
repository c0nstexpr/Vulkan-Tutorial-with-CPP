#pragma once

namespace vulkan::utility
{
    constexpr gltf_model::sampler::sampler(const tinygltf::Sampler& sampler) noexcept:
        mag_filter(get_vk_filter_from_gltf(sampler.magFilter)),
        min_filter(get_vk_filter_from_gltf(sampler.minFilter)),
        address_mode_u(get_vk_addr_node_from_gltf(sampler.wrapS)),
        address_mode_v(get_vk_addr_node_from_gltf(sampler.wrapT)),
        address_mode_w(address_mode_v) {}

    constexpr Filter gltf_model::sampler::get_vk_filter_from_gltf(const int gltf_filter)
    {
        switch(gltf_filter)
        {
        case TINYGLTF_TEXTURE_FILTER_NEAREST:
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST: return Filter::eNearest;
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
        case TINYGLTF_TEXTURE_FILTER_LINEAR: return Filter::eLinear;
        default: throw std::invalid_argument{"unknown filter"};
        }
    }

    constexpr SamplerAddressMode gltf_model::sampler::get_vk_addr_node_from_gltf(const int gltf_wrap_node)
    {
        switch(gltf_wrap_node)
        {
        case TINYGLTF_TEXTURE_WRAP_REPEAT: return SamplerAddressMode::eRepeat;
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE: return SamplerAddressMode::eClampToEdge;
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT: return SamplerAddressMode::eMirroredRepeat;
        default: throw std::invalid_argument{"unknown node mode"};
        }
    }
}
