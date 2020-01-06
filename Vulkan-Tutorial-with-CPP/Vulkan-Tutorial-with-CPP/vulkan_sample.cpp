#include "vulkan_sample.h"
#include <unordered_map>

namespace vulkan
{
    const string vulkan_sample::window_title = "vulkan";

    void vulkan_sample::initialize_window() noexcept
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window_ = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
    }

    void vulkan_sample::generate_debug_messenger_create_info()
    {
        using debug_messenger_type = decltype(debug_messenger_);
        using debug_messenger_info_type = debug_messenger_type::info_type;
        debug_messenger_ = debug_messenger_object{
            {
                {},
                {
                    DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                    DebugUtilsMessageSeverityFlagBitsEXT::eError
                },
                {
                    DebugUtilsMessageTypeFlagBitsEXT::eGeneral | DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                    DebugUtilsMessageTypeFlagBitsEXT::eValidation
                },
                debug_callback
            }
        };
    }

    void vulkan_sample::generate_instance_create_info()
    {
        using instance_type = decltype(instance_);
        using instance_info_type = instance_type::info_type;
        vector<string> ext_names;
        vector<string> layer_names;
        InstanceCreateInfo info;
        {
            uint32_t count;
            const auto extensions = glfwGetRequiredInstanceExtensions(&count);
            ext_names.resize(count);
            std::transform(
                extensions,
                extensions + count,
                ext_names.begin(),
                [&](const auto& str)mutable { return str; }
            );
        }
        if constexpr(is_debug)
        {
            layer_names.push_back("VK_LAYER_KHRONOS_validation");
            if(!is_included(
                layer_names,
                enumerateInstanceLayerProperties(DispatchLoaderStatic{}),
                [](const auto& layer, const auto& p)-> bool { return layer == p.layerName; }
            ))
                throw std::runtime_error("validation layers requested, but not available!");
            generate_debug_messenger_create_info();
            info.pNext = &debug_messenger_.info();
            ext_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        if(!is_included(
            ext_names,
            enumerateInstanceExtensionProperties(nullptr, DispatchLoaderStatic{}),
            [](const auto& layer, const auto& p)-> bool { return layer == p.extensionName; }
        ))
            throw std::runtime_error("requested instance extension is not available!");
        instance_ = instance_type{
            instance_info_type{
                info_proxy<ApplicationInfo>{"Hello Vulkan", "No Engine"},
                std::move(ext_names),
                std::move(layer_names),
                std::move(info)
            }
        };
    }

    void vulkan_sample::initialize_instance()
    {
        generate_instance_create_info();
        instance_.initialize();
    }

    void vulkan_sample::generate_surface_create_info()
    {
        using surface_type = decltype(surface_);
        using surface_info_type = surface_type::info_type;
        surface_ = surface_type{surface_info_type{window_}};
    }

    void vulkan_sample::initialize_debug_messenger() { if constexpr(is_debug) debug_messenger_.initialize(instance_); }

    void vulkan_sample::initialize_surface()
    {
        generate_surface_create_info();
        surface_.initialize(instance_);
    }

    bool vulkan_sample::generate_physical_device(
        const PhysicalDevice& physical_device,
        const surface_object& surface_object
    )
    {
        if(physical_device.getProperties(instance_.dispatch()).deviceType == PhysicalDeviceType::eDiscreteGpu)
        {
            size_t i = 0;
            for(const auto& p : physical_device.getQueueFamilyProperties(instance_.dispatch()))
                if(p.queueCount != 0)
                {
                    bool done = true;
                    if(graphics_queue_index_ == queue_family_ignore<>)
                        if(p.queueFlags & QueueFlagBits::eGraphics) graphics_queue_index_ = static_cast<uint32_t>(i);
                        else done = false;
                    if(present_queue_index_ == queue_family_ignore<>)
                        if(physical_device.getSurfaceSupportKHR(
                            static_cast<uint32_t>(i),
                            *surface_object,
                            instance_.dispatch()
                        ))
                            present_queue_index_ = static_cast<uint32_t>(i);
                        else done = false;
                    if(done) return true;
                    ++i;
                }
        }
        return false;
    }

    void vulkan_sample::initialize_physical_device()
    {
        physical_device_ = {*instance_, [this](const auto& d) { return generate_physical_device(d, surface_); }};
    }

    void vulkan_sample::generate_device_create_info()
    {
        using device_type = decltype(device_);
        using device_info_type = device_type::info_type;
        using set_type = decay_to_origin_t<decltype(device_.info().queue_create_infos_set_property())>;
        PhysicalDeviceFeatures features;
        features.samplerAnisotropy = true;
        device_ = device_type{
            device_info_type{
                {
                    info_proxy<DeviceQueueCreateInfo>{{1}, {{}, graphics_queue_index_}},
                    info_proxy<DeviceQueueCreateInfo>{{1}, {{}, graphics_queue_index_}}
                },
                vector<string>{VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                features
            }
        };
        if(!is_included(
            device_.info().extension_name_strs_property(),
            physical_device_->enumerateDeviceExtensionProperties(nullptr, DispatchLoaderStatic{}),
            [](const auto& layer, const auto& p)-> bool { return layer == p.extensionName; }
        ))
            throw std::runtime_error("requested device extension is not available!");
    }

    void vulkan_sample::initialize_device()
    {
        generate_device_create_info();
        device_.initialize(*physical_device_, instance_.dispatch());
    }

    void vulkan_sample::initialize_queue()
    {
        graphics_queue_ = device_->getQueue(graphics_queue_index_, 0, device_.dispatch());
        present_queue_ = device_->getQueue(present_queue_index_, 0, device_.dispatch());
    }

    void vulkan_sample::generate_model()
    {
        string err, warn;
        const auto& model_path = path{"resource"} / "room" / "Enter a title.obj";

        if(!LoadObj(
            &model_.attribute,
            &model_.shapes,
            &model_.materials,
            &err,
            &warn,
            model_path.u8string().c_str()
        ))
            throw std::runtime_error{err + warn};
        std::cout << warn;
    }

    void vulkan_sample::generate_shader_module_create_infos()
    {
        using shader_module_type = decltype(vertex_shader_module_);
        using shader_module_info_type = shader_module_type::info_type;

        const auto& shaders_path = path{"resource"} / "shaders";
        const auto& vertex_shader_code_path = shaders_path / "shader.vert";
        const auto& fragment_shader_code_path = shaders_path / "shader.frag";
        CompileOptions options;
        options.SetGenerateDebugInfo();
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        cfin.open(vertex_shader_code_path);
        if(!cfin) throw std::runtime_error("failed to load vertex code file\n");
        csout << cfin.rdbuf();
        {
            auto&& [spriv_code, error_str, status] = glsl_compile_to_spriv(
                csout.str(),
                shaderc_vertex_shader,
                "vertex",
                options
            );
            if(status != shaderc_compilation_status_success)
                throw std::runtime_error(
                    "vertex code compile failure\n" + error_str
                );
            vertex_shader_module_ = shader_module_type{shader_module_info_type{std::move(spriv_code), {}}};
        }
        cfin.close();

        cfin.open(fragment_shader_code_path);
        if(!cfin) throw std::runtime_error("failed to load fragment code file\n");
        csout.str("");
        csout << cfin.rdbuf();
        {
            auto&& [spriv_code, error_str, status] = glsl_compile_to_spriv(
                csout.str(),
                shaderc_fragment_shader,
                "fragment",
                options
            );
            if(status != shaderc_compilation_status_success)
                throw std::runtime_error(
                    "fragment code compile failure\n" + error_str
                );
            fragment_shader_module_ = shader_module_type{shader_module_info_type{std::move(spriv_code)}};
        }
        cfin.close();
    }

    void vulkan_sample::generate_descriptor_set_layout_create_info()
    {
        using descriptor_set_layout_type = decltype(descriptor_set_layout_);
        using descriptor_set_layout_info_type = descriptor_set_layout_type::info_type;
        descriptor_set_layout_ = descriptor_set_layout_type{
            info_proxy<DescriptorSetLayoutCreateInfo>{
                {
                    DescriptorSetLayoutBinding{0, DescriptorType::eUniformBuffer, 1, ShaderStageFlagBits::eVertex},
                    DescriptorSetLayoutBinding{
                        1,
                        DescriptorType::eCombinedImageSampler,
                        1,
                        ShaderStageFlagBits::eFragment
                    }
                }
            }
        };
    }

    map<string, stb::image<channel::rgb_alpha>> vulkan_sample::generate_texture_image_create_info()
    {
        const auto& directory = path{"resource"} / "room";
        const auto& paths = {
            directory / "Alyscamps_Arles_dos.jpg",
            directory / "bad1.jpg",
            directory / "belye.jpg",
            directory / "chair1.jpg",
            directory / "doors1.jpg",
            directory / "floor1.jpg",
            directory / "glass_left.png",
            directory / "glass_right.png",
            directory / "rest.jpg",
            directory / "table.jpg",
            directory / "veshalka.jpg",
            directory / "wall_staff.jpg",
            directory / "win_left.jpg",
            directory / "win_right.jpg"
        };

        map<string, stb::image<channel::rgb_alpha>> image_sources;

        for(const auto& path : paths)
            image_sources[path.generic_u8string()] = stb::image<channel::rgb_alpha>{path};

        return image_sources;
    }

    pair<vector<vertex>, vector<uint32_t>> vulkan_sample::generate_buffer_allocate_info()
    {
        struct vertex_hasher
        {
            size_t operator()(const vertex& v)const
            {
                const auto hash_pos = std::hash<decltype(v.pos)>{}(v.pos);
                const auto hash_color = std::hash<decltype(v.color)>{}(v.color);
                const auto hash_tex_coordinate = std::hash<decltype(v.texture_coordinate)>{}(v.texture_coordinate);

                return hash_pos ^ hash_color << 1 ^ hash_tex_coordinate << 2;
            }
        };

        vector<uint32_t> indices;

        unordered_map<vertex,uint32_t,vertex_hasher> vertices_map;
        vector<vertex> vertices;

        for(const auto& shape : model_.shapes)
            for(const auto& index : shape.mesh.indices)
            {
                vertex vertex = {
                    {
                        model_.attribute.vertices[3 * index.vertex_index + 0],
                        model_.attribute.vertices[3 * index.vertex_index + 1],
                        model_.attribute.vertices[3 * index.vertex_index + 2]
                    },
                    vec3{1},
                    {
                        model_.attribute.texcoords[2 * index.texcoord_index],
                        1 - model_.attribute.texcoords[2 * index.texcoord_index + 1]
                    }
                };
                auto&& it = vertices_map.find(vertex);
                if(it == vertices_map.cend())
                {
                    it = vertices_map.insert({vertex, static_cast<decltype(vertices_map)::mapped_type>(vertices_map.size())}).first;
                    vertices.push_back(vertex);
                }
                indices.push_back(it->second);
            }
        transfer_memory_ = decltype(transfer_memory_){
            *physical_device_,
            device_,
            array<BufferUsageFlags, 2>{BufferUsageFlagBits::eVertexBuffer, BufferUsageFlagBits::eIndexBuffer},
            {vertices.size(),indices.size()}
        };

        return {vertices,indices};
    }

    void vulkan_sample::generate_texture_sampler_create_info()
    {
        using sampler_type = decltype(texture_sampler_);
        using sampler_info_type = sampler_type::info_type;

        sampler_info_type info;
        info.magFilter = info.minFilter = Filter::eLinear;
        info.mipmapMode = SamplerMipmapMode::eLinear;
        info.anisotropyEnable = true;
        info.maxAnisotropy = decltype(texture_images_)::mapped_type::max_anisotropy;
        info.compareOp = CompareOp::eAlways;
        info.borderColor = BorderColor::eIntOpaqueBlack;

        texture_sampler_ = sampler_type{std::move(info)};
    }

    void vulkan_sample::generate_transform_buffer_create_info()
    {
        using uniform_buffer_type = decltype(transform_buffer_);
        using uniform_buffer_info_type = uniform_buffer_type::info_type;
        transform_buffer_ = uniform_buffer_type{
            uniform_buffer_info_type{{}, {{}, sizeof(transform), BufferUsageFlagBits::eUniformBuffer}}
        };
    }

    void vulkan_sample::generate_graphics_command_pool_create_info()
    {
        using command_pool_type = decltype(graphics_command_pool_);
        using command_pool_info_type = command_pool_type::info_type;
        graphics_command_pool_ = command_pool_type{
            command_pool_info_type{CommandPoolCreateFlagBits::eResetCommandBuffer, graphics_queue_index_}
        };
    }

    void vulkan_sample::initialize_shader_module()
    {
        generate_shader_module_create_infos();
        vertex_shader_module_.initialize(device_);
        fragment_shader_module_.initialize(device_);
    }

    void vulkan_sample::initialize_descriptor_set_layout()
    {
        generate_descriptor_set_layout_create_info();
        descriptor_set_layout_.initialize(device_);
    }

    void vulkan_sample::initialize_texture_image()
    {
        auto&& image_sources = generate_texture_image_create_info();

        for(auto& source : image_sources)
        {
            auto&& texture_image = decltype(texture_images_)::mapped_type{
                ImageType::e2D,
                {static_cast<uint32_t>(source.second.width()), static_cast<uint32_t>(source.second.height()), 1}
            };
            texture_image.initialize(device_, *physical_device_);
            texture_image.write_from_src(device_, source.second.cbegin(), source.second.cend());
            texture_images_[source.first] = std::move(texture_image);
        }
    }

    void vulkan_sample::initialize_buffer()
    {
        auto&& [vertices,indices] = generate_buffer_allocate_info();
        transfer_memory_.initialize(*physical_device_);
        transfer_memory_.write(std::move(vertices));
        transfer_memory_.write(std::move(indices));
    }

    void vulkan_sample::initialize_texture_sampler() { texture_sampler_.initialize(device_); }

    void vulkan_sample::initialize_transform_buffer()
    {
        generate_transform_buffer_create_info();
        transform_buffer_.initialize(device_);
        transform_buffer_memory_ = generate_buffer_memory_info(
            device_,
            {*transform_buffer_},
            *physical_device_,
            MemoryPropertyFlagBits::eHostVisible
        ).first;
        transform_buffer_memory_.initialize(device_);
        device_->bindBufferMemory(*transform_buffer_, *transform_buffer_memory_, 0, device_.dispatch());
        set_transform({mat4{1}});
    }

    void vulkan_sample::initialize_graphics_command_pool()
    {
        generate_graphics_command_pool_create_info();
        graphics_command_pool_.initialize(device_);
    }

    void vulkan_sample::generate_pipeline_layout_create_info(
        const descriptor_set_layout_object& descriptor_set_layout_object
    )
    {
        using pipeline_layout_type = decltype(pipeline_layout_);
        using pipeline_layout_info_type = pipeline_layout_type::info_type;
        pipeline_layout_ = pipeline_layout_type{pipeline_layout_info_type{{*descriptor_set_layout_object}}};
    }

    void vulkan_sample::generate_swapchain_create_info(const surface_object& surface_object)
    {
        using swapchain_type = decltype(swapchain_);
        using swapchain_info_type = decltype(swapchain_)::info_type;
        using set_type = decay_to_origin_t<decltype(swapchain_.info().queue_family_indices_set_property())>;
        const auto& formats = physical_device_->getSurfaceFormatsKHR(*surface_object, DispatchLoaderStatic{});
        const SurfaceFormatKHR required_format = []()
        {
            SurfaceFormatKHR format_khr;
            format_khr.format = Format::eB8G8R8A8Unorm;
            format_khr.colorSpace = ColorSpaceKHR::eSrgbNonlinear;
            return format_khr;
        }();
        const auto available_format = std::find(formats.cbegin(), formats.cend(), required_format) == formats.end() ?
            formats.front() :
            required_format;
        const auto& present_modes = physical_device_->getSurfacePresentModesKHR(
            *surface_object,
            DispatchLoaderStatic{}
        );
        static constexpr auto required_present_mode = PresentModeKHR::eFifo;
        const auto present_mode = std::find(present_modes.cbegin(), present_modes.cend(), required_present_mode) !=
            present_modes.cend() ?
            required_present_mode :
            PresentModeKHR::eFifo;
        const auto& capabilities = physical_device_->getSurfaceCapabilitiesKHR(*surface_object, DispatchLoaderStatic{});
        const auto extent = capabilities.currentExtent.width == -1 ?
            Extent2D{width, height} :
            Extent2D{
                std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };
        auto sharing_mode = SharingMode::eConcurrent;
        set_type queue_indices;
        if(device_.info().queue_create_infos_set_property().size() > 1)
            for(const auto& info : device_.info().queue_create_infos_set_property())
                queue_indices.insert(
                    info.info.queueFamilyIndex
                );
        else sharing_mode = SharingMode::eExclusive;
        swapchain_ = swapchain_type{
            swapchain_info_type{
                std::move(queue_indices),
                SwapchainCreateInfoKHR{
                    {},
                    *surface_,
                    std::min(capabilities.minImageCount + 1, capabilities.maxImageCount),
                    available_format.format,
                    available_format.colorSpace,
                    extent,
                    1,
                    ImageUsageFlagBits::eColorAttachment,
                    sharing_mode,
                    0,
                    nullptr,
                    capabilities.currentTransform,
                    CompositeAlphaFlagBitsKHR::eOpaque,
                    present_mode,
                    true,
                }
            }
        };
    }

    void vulkan_sample::initialize_pipeline_layout()
    {
        generate_pipeline_layout_create_info(descriptor_set_layout_);
        pipeline_layout_.initialize(device_);
    }

    void vulkan_sample::initialize_swapchain()
    {
        generate_swapchain_create_info(surface_);
        swapchain_.initialize(device_);
    }

    void vulkan_sample::generate_depth_image_create_info(const swapchain_object& swapchain_object)
    {
        const auto format = [this](
            const vector<Format>& formats,
            const ImageTiling tiling,
            const FormatFeatureFlags feature
        )
            {
                for(const auto& f : formats)
                {
                    const auto& format_props = physical_device_->getFormatProperties(f, device_.dispatch());
                    switch(tiling)
                    {
                    case ImageTiling::eOptimal: if((format_props.optimalTilingFeatures & feature) == feature) return f;
                        break;
                    case ImageTiling::eLinear: if((format_props.linearTilingFeatures & feature) == feature) return f;
                        break;
                    default: break;
                    }
                }
                throw std::runtime_error("failed to get supported format");
            }(
                {Format::eD32Sfloat, Format::eD32SfloatS8Uint, Format::eD24UnormS8Uint},
                ImageTiling::eOptimal,
                FormatFeatureFlagBits::eDepthStencilAttachment
            );
        depth_image_ = {format, ImageType::e2D, Extent3D{swapchain_object.info().info.imageExtent, 1}};
    }

    void vulkan_sample::generate_image_view_create_infos(const swapchain_object& swapchain_object)
    {
        using image_view_type = decltype(image_views_)::value_type;
        auto&& images = device_->getSwapchainImagesKHR(*swapchain_object, device_.dispatch());
        image_views_.resize(images.size());
        std::transform(
            images.begin(),
            images.end(),
            image_views_.begin(),
            [this, format = swapchain_object.info().info.imageFormat](decltype(*images.begin()) image)mutable
            {
                return image_view_type{
                    {
                        {},
                        std::move(image),
                        ImageViewType::e2D,
                        format,
                        ComponentMapping{},
                        ImageSubresourceRange{ImageAspectFlagBits::eColor, 0, 1, 0, 1}
                    }
                };
            }
        );
    }

    void vulkan_sample::initialize_depth_image()
    {
        generate_depth_image_create_info(swapchain_);
        depth_image_.initialize(device_, *physical_device_);
    }

    void vulkan_sample::initialize_image_views()
    {
        generate_image_view_create_infos(swapchain_);
        for(auto& view : image_views_) view.initialize(device_);
    }

    void vulkan_sample::generate_graphics_command_buffer_allocate_info(
        const vector<image_view_object>& image_view_objects,
        const command_pool_object& command_pool_object
    )
    {
        using command_buffer_type = decltype(graphics_command_buffers_)::value_type;
        using command_buffer_info_type = command_buffer_type::info_type;
        graphics_command_buffers_.resize(image_view_objects.size());
        for(auto& buffer : graphics_command_buffers_)
            buffer = command_buffer_type{
                {
                    *command_pool_object,
                    CommandBufferLevel::ePrimary,
                    static_cast<decltype(buffer.info().commandBufferCount)>(graphics_command_buffers_.size())
                }
            };
    }

    void vulkan_sample::generate_render_pass_create_info(
        const swapchain_object& swapchain_object,
        const depth_image& depth_image
    )
    {
        using render_pass_type = decltype(render_pass_);
        using render_pass_info_type = decltype(render_pass_)::info_type;

        //the first subpass deal with the color attachment
        AttachmentDescription color_attachment = {
            {},
            swapchain_object.info().info.imageFormat,
            SampleCountFlagBits::e1,
            AttachmentLoadOp::eClear,
            AttachmentStoreOp::eStore,
            AttachmentLoadOp::eDontCare,
            AttachmentStoreOp::eDontCare,
            ImageLayout::eUndefined,
            ImageLayout::ePresentSrcKHR
        };
        AttachmentDescription depth_attachment = {
            {},
            depth_image.image().info().info.format,
            depth_image.image().info().info.samples,
            AttachmentLoadOp::eClear,
            AttachmentStoreOp::eDontCare,
            AttachmentLoadOp::eDontCare,
            AttachmentStoreOp::eDontCare,
            ImageLayout::eUndefined,
            ImageLayout::eDepthStencilAttachmentOptimal
        };
        AttachmentReference color_attachment_ref = {0, ImageLayout::eColorAttachmentOptimal};
        AttachmentReference depth_attachment_ref = {1, depth_attachment.finalLayout};
        auto subpass = info_proxy<SubpassDescription>{
            {},
            vector<AttachmentReference>{std::move(color_attachment_ref)},
            vector<AttachmentReference>{},
            std::move(depth_attachment_ref)
        };
        SubpassDependency dependency = {
            subpass_external<decltype(dependency.srcSubpass)>,
            0,
            PipelineStageFlagBits::eColorAttachmentOutput,
            PipelineStageFlagBits::eColorAttachmentOutput,
            AccessFlags{},
            AccessFlagBits::eColorAttachmentRead | AccessFlagBits::eColorAttachmentWrite,
        };
        render_pass_ = render_pass_type{
            render_pass_info_type{
                {std::move(color_attachment), std::move(depth_attachment)},
                vector<info_proxy<SubpassDescription>>{std::move(subpass)},
                vector<SubpassDependency>{std::move(dependency)}
            }
        };
    }

    void vulkan_sample::generate_descriptor_pool_create_info(const vector<image_view_object>& image_view_objects)
    {
        using descriptor_pool_type = decltype(descriptor_pool_);
        using descriptor_pool_info_type = descriptor_pool_type::info_type;
        descriptor_pool_ = descriptor_pool_type{
            descriptor_pool_info_type{
                {
                    DescriptorPoolSize
                    {DescriptorType::eUniformBuffer, static_cast<uint32_t>(image_view_objects.size())},
                    DescriptorPoolSize{
                        DescriptorType::eCombinedImageSampler,
                        static_cast<uint32_t>(image_view_objects.size())
                    }
                },
                descriptor_pool_info_type::base_info_type{DescriptorPoolCreateFlagBits::eFreeDescriptorSet}
            }
        };
    }

    void vulkan_sample::generate_sync_objects_create_info(const vector<image_view_object>& image_view_objects)
    {
        using semaphore_type = decltype(swapchain_image_syn_)::value_type;
        using semaphore_info_type = semaphore_type::info_type;
        using fence_type = decltype(gpu_syn_)::value_type;
        using fence_info_type = fence_type::info_type;
        swapchain_image_syn_.resize(image_view_objects.size());
        render_syn_.resize(image_view_objects.size());
        gpu_syn_.resize(image_view_objects.size());
        for(auto& syn : gpu_syn_) syn = fence_type{fence_info_type{FenceCreateFlagBits::eSignaled}};
    }

    void vulkan_sample::initialize_graphics_command_buffer()
    {
        generate_graphics_command_buffer_allocate_info(image_views_, graphics_command_pool_);
        graphics_command_buffers_ = graphics_command_pool_.create_element_objects(
            device_,
            graphics_command_buffers_.front().info()
        );
    }

    void vulkan_sample::initialize_render_pass()
    {
        generate_render_pass_create_info(swapchain_, depth_image_);
        render_pass_.initialize(device_);
    }

    void vulkan_sample::initialize_descriptor_pool()
    {
        generate_descriptor_pool_create_info(image_views_);
        descriptor_pool_.initialize(device_);
    }

    void vulkan_sample::initialize_sync_objects()
    {
        generate_sync_objects_create_info(image_views_);
        for(auto& semaphore : swapchain_image_syn_) semaphore.initialize(device_);
        for(auto& semaphore : render_syn_) semaphore.initialize(device_);
        for(auto& fence : gpu_syn_) fence.initialize(device_);
    }

    void vulkan_sample::generate_framebuffer_create_infos(
        const vector<image_view_object>& image_view_objects,
        const depth_image& depth_image,
        const render_pass_object& render_pass_object,
        const swapchain_object& swapchain_object
    )
    {
        using frame_buffer_type = decltype(frame_buffers_)::value_type;
        using frame_buffer_info_type = frame_buffer_type::info_type;
        frame_buffers_.resize(image_view_objects.size());
        std::transform(
            image_view_objects.cbegin(),
            image_view_objects.cend(),
            frame_buffers_.begin(),
            [ this, extent = swapchain_object.info().info.imageExtent, render_pass = *render_pass_object,
                depth_image_view = *depth_image.image_view() ](const image_view_object& image_view)
            {
                return frame_buffer_type{
                    frame_buffer_info_type{
                        {*image_view, depth_image_view},
                        frame_buffer_info_type::base_info_type{
                            {},
                            render_pass,
                            0,
                            nullptr,
                            extent.width,
                            extent.height,
                            1
                        }
                    }
                };
            }
        );
    }

    void vulkan_sample::generate_graphics_pipeline_create_info(
        const shader_module_object& vertex_shader_module_object,
        const shader_module_object& fragment_shader_module_object,
        const swapchain_object& swapchain_object,
        const render_pass_object& render_pass_object,
        const pipeline_layout_object& pipeline_layout_object
    )
    {
        using graphics_pipeline_type = decltype(graphics_pipeline_);
        using graphics_pipeline_info_type = graphics_pipeline_type::info_type;

        GraphicsPipelineCreateInfo info;
        auto vertex_shader_stage = info_proxy<PipelineShaderStageCreateInfo>{
            "main",
            nullopt,
            PipelineShaderStageCreateInfo{{}, ShaderStageFlagBits::eVertex, *vertex_shader_module_object}
        };
        auto fragment_shader_stage = info_proxy<PipelineShaderStageCreateInfo>{
            "main",
            nullopt,
            PipelineShaderStageCreateInfo{{}, ShaderStageFlagBits::eFragment, *fragment_shader_module_object}
        };
        auto input_state = info_proxy<PipelineVertexInputStateCreateInfo>{
            {vertex::description},
            {vertex::attribute_descriptions.cbegin(), vertex::attribute_descriptions.cend()}
        };
        PipelineInputAssemblyStateCreateInfo input_assembly_state = {{}, PrimitiveTopology::eTriangleList};
        auto viewport_state = info_proxy<PipelineViewportStateCreateInfo>{
            {
                {
                    0,
                    0,
                    static_cast<float>(swapchain_object.info().info.imageExtent.width),
                    static_cast<float>(swapchain_object.info().info.imageExtent.height),
                    0,
                    1
                }
            },
            vector<Rect2D>{Rect2D{{0, 0}, swapchain_object.info().info.imageExtent}}
        };
        PipelineRasterizationStateCreateInfo rasterization_state = {
            {},
            false,
            false,
            PolygonMode::eFill,
            CullModeFlagBits::eBack,
            FrontFace::eCounterClockwise,
            false,
            0,
            0,
            0,
            1
        };
        PipelineDepthStencilStateCreateInfo depth_stencil_state = {{}, true, true, CompareOp::eLess};
        auto color_blend_state = info_proxy<PipelineColorBlendStateCreateInfo>{
            {
                {
                    false,
                    BlendFactor::eZero,
                    BlendFactor::eZero,
                    BlendOp::eAdd,
                    BlendFactor::eZero,
                    BlendFactor::eZero,
                    BlendOp::eAdd,
                    ColorComponentFlags{
                        ColorComponentFlagBits::eA | ColorComponentFlagBits::eR | ColorComponentFlagBits::eG |
                        ColorComponentFlagBits::eB
                    }
                }
            },
            PipelineColorBlendStateCreateInfo{{}, false, LogicOp::eCopy, 0, nullptr, std::array<float, 4>{0}}
        };

        info.renderPass = *render_pass_object;
        info.layout = *pipeline_layout_object;
        graphics_pipeline_ = graphics_pipeline_type{
            graphics_pipeline_info_type{
                {std::move(vertex_shader_stage), std::move(fragment_shader_stage)},
                {std::move(input_state)},
                {std::move(input_assembly_state)},
                {nullopt},
                {std::move(viewport_state)},
                {std::move(rasterization_state)},
                {{}},
                {std::move(depth_stencil_state)},
                {std::move(color_blend_state)},
                {nullopt},
                {std::move(info)}
            }
        };
    }

    void vulkan_sample::generate_descriptor_set_allocate_info(
        const vector<image_view_object>& image_view_objects,
        const descriptor_set_layout_object& descriptor_set_layout_object,
        const descriptor_pool_object& descriptor_pool_object
    )
    {
        using descriptor_set_type = decltype(descriptor_sets_)::value_type;
        using descriptor_set_info_type = descriptor_set_type::info_type;

        descriptor_sets_.resize(image_view_objects.size());
        for(auto& descriptor_set : descriptor_sets_)
            descriptor_set = descriptor_set_type{
                descriptor_set_info_type{
                    {image_view_objects.size(), *descriptor_set_layout_object},
                    descriptor_set_info_type::base_info_type{*descriptor_pool_object}
                }
            };
    }

    void vulkan_sample::initialize_frame_buffer()
    {
        generate_framebuffer_create_infos(image_views_, depth_image_, render_pass_, swapchain_);
        for(auto& fb : frame_buffers_) fb.initialize(device_);
    }

    void vulkan_sample::initialize_graphics_pipeline()
    {
        generate_graphics_pipeline_create_info(
            vertex_shader_module_,
            fragment_shader_module_,
            swapchain_,
            render_pass_,
            pipeline_layout_
        );
        graphics_pipeline_.initialize(device_);
    }

    void vulkan_sample::initialize_descriptor_sets()
    {
        generate_descriptor_set_allocate_info(image_views_, descriptor_set_layout_, descriptor_pool_);
        descriptor_sets_ = descriptor_pool_.create_element_objects(device_, descriptor_sets_.front().info().info);
    }

    void vulkan_sample::submit_precondition_command()
    {
        const auto& front_command_buffer = *graphics_command_buffers_.front();
        auto& front_submit_info = submit_infos_.front();

        front_submit_info = {};
        front_submit_info.command_buffers_property = {front_command_buffer};

        command_buffer_begin_info_ = CommandBufferBeginInfo{CommandBufferUsageFlagBits::eOneTimeSubmit};

        front_command_buffer.begin(command_buffer_begin_info_, device_.dispatch());

        transfer_memory_.write_transfer_command(front_command_buffer);

        for(auto& pair : texture_images_)
        {
            const auto& texture_image = pair.second;
            texture_image.write_transfer_command(device_, front_command_buffer);

            write_transfer_image_layout_command(
                front_command_buffer,
                *texture_image.image(),
                texture_image.image_view().info().subresourceRange,
                ImageLayout::eTransferDstOptimal,
                ImageLayout::eShaderReadOnlyOptimal,
                device_.dispatch()
            );

            device_->flushMappedMemoryRanges(
                MappedMemoryRange{*texture_image.buffer_memory(), 0, constant::whole_size<DeviceSize>},
                device_.dispatch()
            );
        }

        front_command_buffer.end(device_.dispatch());

        graphics_queue_.submit({front_submit_info}, nullptr, device_.dispatch());
    }

    void vulkan_sample::generate_render_info()
    {
        render_pass_begin_infos_.resize(frame_buffers_.size());
        submit_infos_.resize(graphics_command_buffers_.size());
        present_infos_.resize(submit_infos_.size());
        submit_precondition_command();

        for(const auto& descriptor_set : descriptor_sets_)
            device_->updateDescriptorSets(
                {
                    info_proxy<WriteDescriptorSet>{
                        {},
                        {{*transform_buffer_, 0, whole_size<decltype(DescriptorBufferInfo::range)>}},
                        {},
                        {*descriptor_set, 0, 0, 1, DescriptorType::eUniformBuffer}
                    },
                    info_proxy<WriteDescriptorSet>{
                        {{*texture_sampler_, *texture_images_.cbegin()->second.image_view(), ImageLayout::eShaderReadOnlyOptimal}},
                        {},
                        {},
                        {*descriptor_set, 1, 0, 1, DescriptorType::eCombinedImageSampler}
                    }
                },
                {},
                device_.dispatch()
            );

        command_buffer_begin_info_ = CommandBufferBeginInfo{CommandBufferUsageFlagBits::eSimultaneousUse};

        graphics_queue_.waitIdle(device_.dispatch());

        std::for_each(
            graphics_command_buffers_.begin(),
            graphics_command_buffers_.end(),
            [this, index = size_t{0}](command_buffer_object& buffer)mutable
            {
                render_pass_begin_infos_[index] = info_proxy<RenderPassBeginInfo>{
                    {ClearColorValue{}, ClearDepthStencilValue{1, 1}},
                    RenderPassBeginInfo{
                        RenderPass{*render_pass_},
                        Framebuffer{*frame_buffers_[index]},
                        Rect2D{{0, 0}, {swapchain_.info().info.imageExtent}}
                    }
                };

                buffer->begin(command_buffer_begin_info_, device_.dispatch());
                buffer->beginRenderPass(render_pass_begin_infos_[index], SubpassContents::eInline, device_.dispatch());

                buffer->bindPipeline(PipelineBindPoint::eGraphics, *graphics_pipeline_, device_.dispatch());
                buffer->bindVertexBuffers(
                    0,
                    {*transfer_memory_.device_local_buffer(vertices_buffer_index)},
                    {0},
                    device_.dispatch()
                );
                buffer->bindIndexBuffer(
                    {*transfer_memory_.device_local_buffer(indices_buffer_index)},
                    {0},
                    index_type<std::decay_t<decltype(get_indices())>::value_type>,
                    device_.dispatch()
                );
                buffer->bindDescriptorSets(
                    PipelineBindPoint::eGraphics,
                    *pipeline_layout_,
                    0,
                    *descriptor_sets_[index],
                    {},
                    device_.dispatch()
                );

                buffer->drawIndexed(static_cast<uint32_t>(get_indices().size()), 1, 0, 0, 0, device_.dispatch());

                buffer->endRenderPass(device_.dispatch());
                buffer->end(device_.dispatch());

                submit_infos_[index] = info_proxy<SubmitInfo>{
                    vector<Semaphore>{},
                    PipelineStageFlagBits::eColorAttachmentOutput,
                    vector<CommandBuffer>{*buffer},
                    vector<Semaphore>{*render_syn_[index]}
                };

                present_infos_[index] = info_proxy<PresentInfoKHR>{
                    submit_infos_[index].signal_semaphores_property,
                    vector<SwapchainKHR>{*swapchain_},
                    vector<uint32_t>{static_cast<uint32_t>(index)}
                };
                index++;
            }
        );
    }

    void vulkan_sample::initialize_vulkan()
    {
        static bool is_initialized = false;
        if(!is_initialized)
        {
            initialize_instance();
            initialize_debug_messenger();
            initialize_surface();
            initialize_physical_device();
            initialize_device();
            initialize_queue();
            generate_model();
            initialize_shader_module();
            initialize_descriptor_set_layout();
            initialize_texture_image();
            initialize_buffer();
            initialize_texture_sampler();
            initialize_transform_buffer();
            initialize_graphics_command_pool();
            is_initialized = true;
        }
        else
        {
            descriptor_pool_.element_objects_reset(device_, descriptor_sets_);
            graphics_pipeline_ = nullptr;
            frame_buffers_.clear();
            swapchain_image_syn_.clear();
            render_syn_.clear();
            gpu_syn_.clear();
            descriptor_pool_ = nullptr;
            render_pass_ = nullptr;
            graphics_command_buffers_.clear();
            image_views_.clear();
            depth_image_ = {};
            pipeline_layout_ = nullptr;
            swapchain_ = nullptr;
        }
        initialize_pipeline_layout();
        initialize_swapchain();
        initialize_depth_image();
        initialize_image_views();
        initialize_graphics_command_buffer();
        initialize_render_pass();
        initialize_descriptor_pool();
        initialize_sync_objects();
        initialize_frame_buffer();
        initialize_graphics_pipeline();
        initialize_descriptor_sets();
        generate_render_info();
        flush_to_memory();
    }

    void vulkan_sample::re_initialize_vulkan()
    {
        {
            int window_width = 0, window_height = 0;
            while((window_width | window_height) == 0)
            {
                glfwGetFramebufferSize(window_, &window_width, &window_height);
                glfwWaitEvents();
            }
        }
        device_->waitIdle(device_.dispatch());
        initialize_vulkan();
    }

    void vulkan_sample::glfw_cleanup() noexcept
    {
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }

    VkBool32 vulkan_sample::debug_callback(
        const VkDebugUtilsMessageSeverityFlagBitsEXT flag_bits,
        const VkDebugUtilsMessageTypeFlagsEXT type_flags,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        [[maybe_unused]] void* data
    )
    {
        const auto flag = DebugUtilsMessageSeverityFlagBitsEXT{flag_bits};
        std::cerr << to_string(flag) << '\n' << to_string(DebugUtilsMessageTypeFlagsEXT{type_flags}) << '\n' <<
            "validation layer: " << p_callback_data->pMessage << '\n';
        return flag == DebugUtilsMessageSeverityFlagBitsEXT::eError ? true : false;
    }

    vulkan_sample::~vulkan_sample() { glfw_cleanup(); }

    auto vulkan_sample::fps() const -> decltype(fps_) { return fps_; }

    const SwapchainCreateInfoKHR& vulkan_sample::swapchain_create_info() const { return swapchain_.info(); }

    void vulkan_sample::initialize()
    {
        initialize_window();
        initialize_vulkan();
    }

    bool vulkan_sample::render()
    {
        static constexpr empty_type empty_type;
        return render(empty_type);
    }

    void vulkan_sample::flush_vertices_to_memory() { transfer_memory_.flush<vertex>(); }

    void vulkan_sample::flush_indices_to_memory() { transfer_memory_.flush<uint32_t>(); }

    void vulkan_sample::flush_transform_to_memory()
    {
        device_->flushMappedMemoryRanges({{*transform_buffer_memory_, 0, whole_size<DeviceSize>}}, device_.dispatch());
    }

    void vulkan_sample::flush_to_memory()
    {
        transfer_memory_.flush<vertex, uint32_t>();
        flush_transform_to_memory();
    }

    void vulkan_sample::set_transform(decltype(transform_mat_) mat)
    {
        transform_mat_ = std::move(mat);
        write(transform_buffer_memory_, device_, transform_mat_);
    }

    void vulkan_sample::set_vertices(decltype(transfer_memory_)::value_type<vertex> vertices)
    {
        transfer_memory_.write<vertex>(std::move(vertices));
    }

    void vulkan_sample::set_indices(decltype(transfer_memory_)::value_type<uint32_t> indices)
    {
        transfer_memory_.write<uint32_t>(std::move(indices));
    }
}
