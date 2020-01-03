#pragma once

#include <vulkan/utility/utility.h>
#include <tiny_gltf.h>
#include <boost/range/adaptor/strided.hpp>

namespace vulkan::utility
{
    class gltf_model
    {
    public:

        struct sampler
        {
            Filter mag_filter = Filter::eLinear;
            Filter min_filter = Filter::eLinear;
            SamplerAddressMode address_mode_u = SamplerAddressMode::eRepeat;
            SamplerAddressMode address_mode_v = SamplerAddressMode::eRepeat;
            SamplerAddressMode address_mode_w = SamplerAddressMode::eRepeat;

            constexpr sampler() noexcept;
            constexpr sampler(const tinygltf::Sampler&) noexcept;

            static constexpr Filter get_vk_filter_from_gltf(const int);
            static constexpr SamplerAddressMode get_vk_addr_node_from_gltf(const int);
        };

        struct texture
        {
            static constexpr auto format = Format::eR8G8B8A8Unorm;
            static constexpr auto image_type = ImageType::e2D;
            static constexpr auto array_layers = 1;

            Extent2D extent;
            uint32_t mip_levels;
            sampler_object::info_type sampler_create_info;
            sampler sampler;

            constexpr texture() noexcept;
            constexpr texture(const tinygltf::Image&) noexcept;
            constexpr texture(const tinygltf::Image&, const tinygltf::Sampler&) noexcept;
        };

        struct material
        {
            enum class alpha_mode
            {
                opaque,
                blend,
                mask
            };

            using material_texture_type = pair<texture, uint8_t>;

            optional<material_texture_type> base_color_texture;
            optional<material_texture_type> metallic_roughness_texture;
            optional<material_texture_type> normal_texture;
            optional<material_texture_type> emissive_texture;
            optional<material_texture_type> occlusion_texture;
            optional<material_texture_type> specular_glossiness_texture;

            optional<texture> diffuse_texture;

            vec4 base_color_factor = vec4{1};
            vec4 emissive_factor= vec4{1};
            vec4 diffuse_factor = vec4{1};

            vec3 specular_factor = vec4{1};

            float roughness_factor = 1;
            float metallic_factor = 1;
            float alpha_cutoff = 1;

            alpha_mode alpha_mode = alpha_mode::opaque;

            constexpr material() noexcept;

            material(const tinygltf::Material&, const vector<texture>&) noexcept;
        };

        struct primitive
        {
            struct vertex
            {
                vec3 position;
                vec3 normal;
                vec2 uv0;
                vec2 uv1;
                vec4 joint;
                vec4 weight;
            };

		    uint32_t index_count;
		    uint32_t vertex_count;
            optional<material> material;

            vector<vertex> vertices;

		    pair<vec3,vec3> bounding;

            constexpr primitive() noexcept;

            primitive(const tinygltf::Primitive&,const tinygltf::Model&) noexcept;
        };

        struct mesh
        {
            vector<primitive> primitives;

            pair<vec3, vec3> get_bounding() const;

            mesh() noexcept;

            mesh(const tinygltf::Mesh&, const tinygltf::Model&);
        };

        struct skin;

        struct node
        {
            int index = -1;

            string name;
            int skin_index = -1;
            vec3 translation;
            vec3 rotation;
            vec3 scale = vec3{1};
            mat4 matrix = mat4{1};

            struct uniform_block
            {
                mat4 matrix;
                array<mat4, 128> joints;
            } ;

            optional<mesh> mesh;

            constexpr node() noexcept;

            node(const int index, const tinygltf::Node&) noexcept;
        };

        struct skin
        {
            string name;
            optional<node> skeleton;
            vector<const node*> joints;
            vector<mat4> inverse_bind_matrices;

            skin() noexcept;

            skin(
                const tinygltf::Skin&,
                const vector<node>&,
                const optional<tuple<const tinygltf::Accessor*, const tinygltf::BufferView*, const tinygltf::Buffer*>>&
            )
            noexcept;
        };


    private:
        tinygltf::Model model_;
        vector<material> materials_;

    public:
        gltf_model(const path&);
    };

}

#include "gltf.tpp"
