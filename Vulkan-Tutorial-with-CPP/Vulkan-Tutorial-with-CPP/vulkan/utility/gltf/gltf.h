#pragma once

#include "vulkan/utility/utility.h"
#include <boost/range/adaptor/strided.hpp>
#include <nlohmann/json.hpp>

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_JSON
#include <tiny_gltf.h>

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

            constexpr sampler() noexcept = default;
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

            constexpr texture() noexcept = default;
            texture(const tinygltf::Image&, const tinygltf::Sampler& = {}) noexcept;
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

            vec4 base_color_factor{1};
            vec4 emissive_factor{1};
            vec4 diffuse_factor{1};

            vec3 specular_factor{1};

            float roughness_factor = 1;
            float metallic_factor = 1;
            float alpha_cutoff = 1;

            alpha_mode alpha_mode = alpha_mode::opaque;

            constexpr material() noexcept = default;

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
            material material{};

            vector<vertex> vertices;
            vector<uint32_t> indices;

            pair<vec3, vec3> bounding;

            constexpr primitive() noexcept = default;

            void initialize_vertices(const tinygltf::Primitive&, const tinygltf::Model&);
            void initialize_indices(const tinygltf::Primitive&, const tinygltf::Model&);
            primitive(const tinygltf::Primitive&, struct material, const tinygltf::Model&);
        };

        struct mesh
        {
            vector<primitive> primitives;

            pair<vec3, vec3> get_bounding() const;

            mesh() noexcept = default;

            mesh(const tinygltf::Mesh&, const tinygltf::Model&, vector<material>);
        };


        struct node;

        struct skin
        {
            string name;
            vector<mat4> inverse_bind_matrices;
            const node* skeleton = nullptr;
            vector<node*> joints;

            skin() noexcept;

            skin(
                const tinygltf::Skin&,
                const optional<
                    tuple<const tinygltf::Accessor*, const tinygltf::BufferView*, const tinygltf::Buffer*>
                >& = nullopt
            )
            noexcept;

            void locate_skeleton(vector<node*>&);

        private:
            size_t skeleton_index_;

        };

        struct node
        {
            int index = -1;

            string name;
            skin skin;
            vec3 translation;
            vec3 rotation;
            vec3 scale = vec3{1};
            mat4 matrix = mat4{1};

            optional<mesh> mesh;

            node* parent = nullptr;
            vector<node> children;

            node() noexcept = default;

            node(
                const int,
                node*,
                vector<struct skin> skins,
                const tinygltf::Node&,
                const tinygltf::Model&,
                vector<material>,
                vector<node*>&
            );
        };


    private:
        tinygltf::Model model_;
        vector<node*> linear_nodes_;

    public:
        gltf_model(const path&);

        mat4 get_dimension()const;
    };
}

#include "gltf.tpp"
