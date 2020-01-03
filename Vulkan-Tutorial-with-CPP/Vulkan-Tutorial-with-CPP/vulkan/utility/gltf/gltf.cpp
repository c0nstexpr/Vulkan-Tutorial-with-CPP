#define TINYGLTF_IMPLEMENTATION
#include "gltf.h"

namespace vulkan::utility
{
    gltf_model::texture::texture(const tinygltf::Image& gltf_image, const tinygltf::Sampler& sampler) noexcept:
        extent(static_cast<uint32_t>(gltf_image.width), static_cast<uint32_t>(gltf_image.height)),
        mip_levels(
            static_cast<uint32_t>(std::floor(std::log2(std::max(extent.width, extent.height))) + 1.0)
        ),
        sampler(sampler) {}

    gltf_model::material::material(const tinygltf::Material& material, const vector<texture>& textures) noexcept
    {
        const auto get_texture_func = [&textures, &material](
            const string& key ,
            const bool from_additional = false
        ) -> optional<material_texture_type>
        {
            const auto& values = from_additional ? material.additionalValues : material.values;
            const auto& iterator = values.find(key);
            return iterator != values.cend() ?
                optional{
                    material_texture_type{textures[iterator->second.TextureIndex()], iterator->second.TextureTexCoord()}
                } :
                nullopt;
        };

        const auto get_factor_func = [&material](
            float& factor,
            const string& key,
            const bool from_additional = false
        )
        {
            const auto& values = from_additional ? material.additionalValues : material.values;
            const auto& iterator = values.find(key);
            if(iterator != values.cend())
                factor = static_cast<float>(iterator->second.Factor());
        };

        const auto get_color_factor_func = [&material](
            vec4& factor,
            const string& key,
            const bool from_additional = false
        )
        {
            const auto& values = from_additional ? material.additionalValues : material.values;
            const auto& iterator = values.find(key);
            if(iterator != values.cend())
                factor = make_vec4(iterator->second.ColorFactor().data());
        };

        for(const auto& item : {
                pair{&base_color_texture, "baseColorTexture"},
                pair{&metallic_roughness_texture, "metallicRoughnessTexture"},
            })
            *item.first = get_texture_func(item.second);

        for(const auto& item : {
                pair{&normal_texture, "normalTexture"},
                pair{&emissive_texture, "emissiveTexture"},
                pair{&occlusion_texture, "occlusionTexture"},
            })
            *item.first = get_texture_func(item.second, true);

        for(const auto& item : {
                pair<float*, string>{&roughness_factor, "roughnessFactor"},
                pair<float*, string>{&metallic_factor, "metallicFactor"}
            })
            get_factor_func(*item.first, item.second);

        for(const auto& item : {
                pair<float*, string>{&alpha_cutoff, "alphaCutoff"}
            })
            get_factor_func(*item.first, item.second, true);

        for(const auto& item : {
                pair<vec4*, string>{&base_color_factor, "baseColorFactor"}
            })
            get_color_factor_func(*item.first, item.second);


        for(const auto& item : {
                pair<vec4*, string>{&emissive_factor, "emissiveFactor"}
            })
            get_color_factor_func(*item.first, item.second, true);

        if(emissive_factor.w != 1)
            emissive_factor.w = 1;
        {
            const auto& iterator = material.additionalValues.find("alphaMode");
            if(iterator != material.additionalValues.end())
                if(iterator->second.string_value == "BLEND") alpha_mode = alpha_mode::blend;
                else if(iterator->second.string_value == "MASK") alpha_mode = alpha_mode::mask;
        }
        const auto& ext = material.extensions.find("KHR_materials_pbrSpecularGlossiness");
        if(ext != material.extensions.end())
        {
            {
                const auto& value = ext->second.Get("specularGlossinessTexture");
                if(value.Type() != tinygltf::Type::NULL_TYPE)
                    specular_glossiness_texture = {
                        textures[value.Get("index").Get<int>()],
                        value.Get("texCoord").Get<int>()
                    };
            }

            {
                const auto& value = ext->second.Get("diffuseTexture");
                if(value.Type() != tinygltf::Type::NULL_TYPE)
                    diffuse_texture = textures[value.Get("index").Get<int>()];
            }

            const auto get_ext_factor_func = [](const tinygltf::Value& src_factor)
            {
                vector<float> target_factor(src_factor.Type() != tinygltf::Type::NULL_TYPE ? src_factor.ArrayLen() : 0);
                std::generate(
                    target_factor.begin(),
                    target_factor.end(),
                    [i = 0, &src_factor]()mutable
                    {
                        auto val = src_factor.Get(i);
                        return val.IsNumber() ?
                            static_cast<float>(val.Get<double>()) :
                            static_cast<float>(val.Get<int>());
                    }
                );
                return target_factor;
            };

            {
                const auto& factor_vec = get_ext_factor_func(ext->second.Get("diffuseFactor"));
                diffuse_factor = make_vec4(factor_vec.data());
            }

            {
                const auto& factor_vec = get_ext_factor_func(ext->second.Get("specularFactor"));
                specular_factor = make_vec3(factor_vec.data());
            }
        }
    }

    void gltf_model::primitive::initialize_vertices(
        const tinygltf::Primitive& gltf_primitive,
        const tinygltf::Model& gltf_model
    )
    {
        vector<decltype(vertex::position)> positions;
        vector<decltype(vertex::normal)> normals;
        vector<decltype(vertex::uv0)> uv0_s;
        vector<decltype(vertex::uv1)> uv1_s;
        vector<decltype(vertex::joint)> joints;
        vector<decltype(vertex::weight)> weights;

        {
            const auto& iterator = gltf_primitive.attributes.find("POSITION");
            if(iterator != gltf_primitive.attributes.cend())
            {
                const auto& accessor = gltf_model.accessors[iterator->second];
                bounding = {make_vec3(accessor.minValues.data()), make_vec3(accessor.maxValues.data())};
            }
        }


        const auto get_value_ptr_func = [&gltf_model,&gltf_primitive](const size_t type_size, const string& str)
        {
            const auto& iterator = gltf_primitive.attributes.find(str);
            if(iterator != gltf_primitive.attributes.cend())
            {
                const auto& accessor = gltf_model.accessors[iterator->second];
                const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
                const auto ptr = &gltf_model.buffers[buffer_view.buffer]
                    .data[accessor.byteOffset + buffer_view.byteOffset];

                return pair{ptr, ptr + accessor.count * type_size} |
                    boost::adaptors::strided(
                        //abandon accessor.ByteStride() to avoid compilation error;
                        [&accessor,&buffer_view]() -> size_t
                        {
                            const auto component_size_in_bytes =
                                tinygltf::GetComponentSizeInBytes(accessor.componentType);
                            const auto type_size_in_bytes = tinygltf::GetTypeSizeInBytes(accessor.type);

                            if(component_size_in_bytes <= 0) { return 0; }

                            return buffer_view.byteStride % component_size_in_bytes ?
                                buffer_view.byteStride :
                                type_size_in_bytes <= 0 ?
                                0 :
                                component_size_in_bytes * type_size_in_bytes;
                        }()
                    );
            }
            return boost::strided_range<const pair<
                decltype(gltf_model.buffers.front().data)::const_pointer,
                decltype(gltf_model.buffers.front().data)::const_pointer>>{0, pair{nullptr, nullptr}};
        };

        {
            const auto& strided_range =
                get_value_ptr_func(sizeof(decltype(positions)::value_type), "POSITION");
            positions = {
                reinterpret_cast<decltype(positions)::const_pointer>(&*strided_range.begin()),
                reinterpret_cast<decltype(positions)::const_pointer>(&*strided_range.end())
            };
        }

        {
            const auto& strided_range = get_value_ptr_func(sizeof(decltype(normals)::value_type), "NORMAL");
            normals = {
                reinterpret_cast<decltype(normals)::const_pointer>(&*strided_range.begin()),
                reinterpret_cast<decltype(normals)::const_pointer>(&*strided_range.end())
            };
        }

        {
            const auto& strided_range = get_value_ptr_func(sizeof(decltype(uv0_s)::value_type), "TEXCOORD_0");
            uv0_s = {
                reinterpret_cast<decltype(uv0_s)::const_pointer>(&*strided_range.begin()),
                reinterpret_cast<decltype(uv0_s)::const_pointer>(&*strided_range.end())
            };
        }

        {
            const auto& strided_range = get_value_ptr_func(sizeof(decltype(uv1_s)::value_type), "TEXCOORD_1");
            uv1_s = {
                reinterpret_cast<decltype(uv1_s)::const_pointer>(&*strided_range.begin()),
                reinterpret_cast<decltype(uv1_s)::const_pointer>(&*strided_range.end())
            };
        }

        {
            const auto& strided_range = get_value_ptr_func(sizeof(decltype(joints)::value_type), "JOINTS_0");
            joints = {
                reinterpret_cast<decltype(joints)::const_pointer>(&*strided_range.begin()),
                reinterpret_cast<decltype(joints)::const_pointer>(&*strided_range.end())
            };
        }

        {
            const auto& strided_range = get_value_ptr_func(sizeof(decltype(weights)::value_type), "WEIGHTS_0");
            weights = {
                reinterpret_cast<decltype(weights)::const_pointer>(&*strided_range.begin()),
                reinterpret_cast<decltype(weights)::const_pointer>(&*strided_range.end())
            };
        }

        {
            const auto sizes = {
                positions.size(),
                normals.size(),
                uv0_s.size(),
                uv1_s.size(),
                joints.size(),
                weights.size(),
            };
            const auto max_size = *std::max_element(sizes.begin(), sizes.end());
            positions.resize(max_size);
            normals.resize(max_size);
            uv0_s.resize(max_size);
            uv1_s.resize(max_size);
            joints.resize(max_size);
            weights.resize(max_size);
            vertices.resize(max_size);
        }


        ::utility::for_each(
            [](
            decltype(vertices)::reference vertex,
            decltype(positions)::reference position,
            decltype(normals)::reference normal,
            decltype(uv0_s)::reference uv0,
            decltype(uv1_s)::reference uv1,
            decltype(joints)::reference joint,
            decltype(weights)::reference weight
        )
            {
                vertex = {
                    position,
                    normal,
                    uv0,
                    uv1,
                    joint,
                    weight
                };
            },
            vertices.begin(),
            vertices.end(),
            positions.begin(),
            normals.begin(),
            uv0_s.begin(),
            uv1_s.begin(),
            joints.begin(),
            weights.begin()
        );
    }

    void gltf_model::primitive::initialize_indices(
        const tinygltf::Primitive& gltf_primitive,
        const tinygltf::Model& gltf_model
    )
    {
        if(gltf_primitive.indices == -1) return;

        const auto& accessor = gltf_model.accessors[gltf_primitive.indices];
        const auto& buffer_view = gltf_model.bufferViews[accessor.bufferView];
        const auto& buffer = gltf_model.buffers[buffer_view.buffer].data;
        const auto& buffer_valid_ptr = &buffer[accessor.byteOffset + buffer_view.byteOffset];

        switch(accessor.componentType)
        {
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
        {
            const auto ptr = reinterpret_cast<const uint32_t*>(buffer_valid_ptr);
            indices = {ptr, ptr + accessor.count};
        }
        break;
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
        {
            const auto ptr = reinterpret_cast<const uint16_t*>(buffer_valid_ptr);
            indices = {ptr, ptr + accessor.count};
        }
        break;
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
        {
            const auto ptr = reinterpret_cast<const uint8_t*>(buffer_valid_ptr);
            indices = {ptr, ptr + accessor.count};
        }
        break;
        default: break;
        }
    }

    gltf_model::primitive::primitive(
        const tinygltf::Primitive& gltf_primitive,
        struct material material,
        const tinygltf::Model& gltf_model
    ):material(std::move(material))
    {
        initialize_vertices(gltf_primitive, gltf_model);
        initialize_indices(gltf_primitive, gltf_model);
    }

    pair<vec3, vec3> gltf_model::mesh::get_bounding() const
    {
        min(vec3{},vec3{});
        return {
            std::min_element(
                primitives.cbegin(),
                primitives.cend(),
                [](decltype(primitives)::const_reference left,decltype(primitives)::const_reference right)
                {
                    return min(left.bounding.first, right.bounding.first) == left.bounding.first ? true : false;
                }
            )->bounding.first,
            std::max_element(
                primitives.cbegin(),
                primitives.cend(),
                [](decltype(primitives)::const_reference left,decltype(primitives)::const_reference right)
                {
                    return max(left.bounding.first, right.bounding.first) == left.bounding.first ? true : false;
                }
            )->bounding.second
        };
    }

    gltf_model::mesh::mesh(
        const tinygltf::Mesh& gltf_mesh,
        const tinygltf::Model& model,
        vector<material> materials
    ):
        primitives(
            ::utility::container_transform<decltype(primitives)>(
                gltf_mesh.primitives,
                [&model, &materials](decltype(gltf_mesh.primitives)::const_reference gltf_primitive)
                {
                    return primitive{gltf_primitive, std::move(materials[gltf_primitive.material]), model};
                }
            )
        ) {}

    gltf_model::node::node(
        const int index,
        const tinygltf::Node& gltf_node,
        const tinygltf::Model& gltf_model,
        vector<material> materials
    ):
        index(index),
        name(gltf_node.name),
        skin_index(gltf_node.skin),
        translation(
            gltf_node.translation.size() == 3 ?
            make_vec3(reinterpret_cast<const decltype(translation)::value_type*>(gltf_node.translation.data())) :
            decltype(translation){}
        ),
        rotation(
            gltf_node.rotation.size() == 3 ?
            make_vec3(reinterpret_cast<const decltype(rotation)::value_type*>(gltf_node.rotation.data())) :
            decltype(rotation){}
        ),
        scale(
            gltf_node.scale.size() == 3 ?
            make_vec3(reinterpret_cast<const decltype(scale)::value_type*>(gltf_node.scale.data())) :
            decltype(scale){}
        ),
        matrix(
            gltf_node.matrix.size() == 16 ?
            make_mat4(reinterpret_cast<const decltype(matrix)::value_type*>(gltf_node.matrix.data())) :
            decltype(matrix){1}
        ),
        mesh({gltf_model.meshes[gltf_node.mesh], gltf_model, std::move(materials)}) {}

    gltf_model::skin::skin() noexcept = default;

    gltf_model::skin::skin(
        const tinygltf::Skin& skin,
        const vector<node>& nodes,
        const optional<tuple<
            const tinygltf::Accessor*,
            const tinygltf::BufferView*,
            const tinygltf::Buffer*>>& matrices_tuple
    ) noexcept :
        name(skin.name),
        skeleton(skin.skeleton != -1 ? optional{nodes[skin.skeleton]} : nullopt),
        joints(skin.joints.size())
    {
        std::transform(
            skin.joints.cbegin(),
            skin.joints.cend(),
            joints.begin(),
            [&nodes](decltype(skin.joints)::const_reference joint) { return &nodes[joint]; }
        );

        if(matrices_tuple)
        {
            const auto begin_ptr = reinterpret_cast<decltype(inverse_bind_matrices)::const_pointer>(
                &std::get<2>(*matrices_tuple)->data[std::get<0>(*matrices_tuple)->byteOffset + std::get<1>(
                    *matrices_tuple
                )->byteOffset]
            );
            inverse_bind_matrices = decltype(inverse_bind_matrices){
                begin_ptr,
                begin_ptr + std::get<0>(*matrices_tuple)->count
            };
        }
    }

    gltf_model::gltf_model(const path& model_path)
    {
        {
            tinygltf::TinyGLTF context;
            pair<std::string, std::string> msg;
            if(context.LoadASCIIFromFile(
                &model_,
                &msg.first,
                &msg.second,
                model_path.generic_u8string()
            ))
                throw std::runtime_error{
                    "failed to load gltf model :" + model_path.string() + "\n" + msg.first + "\n" + msg.second
                };
        }

        const auto& textures = ::utility::container_transform<vector<texture>>(
            model_.textures,
            [this](decltype(model_.textures)::const_reference gltf_texture)
            {
                return gltf_texture.sampler != -1 ?
                    texture{
                        model_.images[gltf_texture.source],
                        model_.samplers[gltf_texture.sampler]
                    } :
                    texture{model_.images[gltf_texture.source]};
            }
        );

        materials_ = ::utility::container_transform<decltype(materials_)>(
            model_.materials,
            [&textures](decltype(model_.materials)::const_reference gltf_material)
            {
                return material{gltf_material, textures};
            }
        );
        // Push a default material at the end of the list for meshes with no material assigned
        materials_.resize(materials_.size() + 1);
    }
}
