#include "vulkan_sample.h"

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
                    DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                    DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            DebugUtilsMessageSeverityFlagBitsEXT::eError
                },
                {
                    DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                    DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
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
            std::transform(extensions, extensions + count, ext_names.begin(), [&](const auto& str)mutable { return str; });
        }

        if constexpr(is_debug)
        {
            layer_names.push_back("VK_LAYER_KHRONOS_validation");
            if(!is_included(
                layer_names,
                enumerateInstanceLayerProperties(DispatchLoaderStatic{}),
                [](const auto& layer, const auto& p)->bool { return layer == p.layerName; }
            ))
                throw std::runtime_error("validation layers requested, but not available!");

            generate_debug_messenger_create_info();
            info.pNext = &debug_messenger_.info();
            ext_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        if(!is_included(
            ext_names,
            enumerateInstanceExtensionProperties(null_opt<const std::string>, DispatchLoaderStatic{}),
            [](const auto& layer, const auto& p)->bool { return layer == p.extensionName; }))
            throw std::runtime_error("requested instance extension is not available!");

        instance_ = instance_type{instance_info_type{
            info_proxy<ApplicationInfo>{"Hello Vulkan", "No Engine"},
            std::move(ext_names),
            std::move(layer_names),
            std::move(info)
        }};
    }

    bool vulkan_sample::generate_physical_device(const PhysicalDevice& physical_device)
    {
        size_t i = 0;
        if(physical_device.getProperties(DispatchLoaderStatic{}).deviceType == PhysicalDeviceType::eDiscreteGpu)
            for(const auto& p : physical_device.getQueueFamilyProperties(DispatchLoaderStatic{}))
            {
                if(p.queueCount == 0) continue;
                bool done = true;

                if(graphics_queue_index_ == -1)
                    if(p.queueFlags & QueueFlagBits::eGraphics)	graphics_queue_index_ = static_cast<uint32_t>(i);
                    else done = false;

                if(present_queue_index_ == -1)
                    if(physical_device.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface_, DispatchLoaderStatic{}))
                        present_queue_index_ = static_cast<uint32_t>(i);
                    else
                        done = false;

                if(done) return true;
                ++i;
            }

        return false;
    }

    void vulkan_sample::generate_device_create_info()
    {
        using device_type = decltype(device_);
        using device_info_type = device_type::info_type;
        using set_type = decay_to_origin_t<decltype(device_.info().queue_create_infos_set_property())>;

        device_ = device_type{device_info_type{
            {
                info_proxy<DeviceQueueCreateInfo>{ { 1 }, {{}, graphics_queue_index_}},
                info_proxy<DeviceQueueCreateInfo>{ { 1 }, {{}, graphics_queue_index_}}
            },
            vector<string>{VK_KHR_SWAPCHAIN_EXTENSION_NAME}}
        };
        if(!is_included(
            device_.info().extension_name_strs_property(),
            physical_device_->enumerateDeviceExtensionProperties(null_opt<const std::string>, DispatchLoaderStatic{}),
            [](const auto& layer, const auto& p)->bool { return layer == p.extensionName; }
        ))
            throw std::runtime_error("requested device extension is not available!");
    }

    void vulkan_sample::generate_surface_create_info()
    {
        using surface_type = decltype(surface_);
        using surface_info_type = surface_type::info_type;

        surface_ = surface_type{surface_info_type{window_}};
    }

    void vulkan_sample::generate_swap_chain_create_info()
    {
        using swap_chain_type = decltype(swap_chain_);
        using swap_chain_info_type = decltype(swap_chain_)::info_type;
        using set_type = decay_to_origin_t<decltype(swap_chain_.info().queue_family_indices_set_property())>;

        const auto& formats = physical_device_->getSurfaceFormatsKHR(*surface_, DispatchLoaderStatic{});
        const SurfaceFormatKHR required_format = []()
        {
            SurfaceFormatKHR format_khr;
            format_khr.format = Format::eB8G8R8A8Unorm;
            format_khr.colorSpace = ColorSpaceKHR::eSrgbNonlinear;
            return format_khr;
        }();
        const auto available_format = std::find(formats.cbegin(), formats.cend(),
            required_format) == formats.end() ? formats.front() : required_format;

        const auto& present_modes = physical_device_->getSurfacePresentModesKHR(*surface_, DispatchLoaderStatic{});
        static constexpr auto required_present_mode = PresentModeKHR::eFifo;
        const auto present_mode =
            std::find(present_modes.cbegin(), present_modes.cend(), required_present_mode) !=
            present_modes.cend() ? required_present_mode : PresentModeKHR::eFifo;

        const auto& capabilities = physical_device_->getSurfaceCapabilitiesKHR(*surface_, DispatchLoaderStatic{});

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
                queue_indices.insert(info.info.queueFamilyIndex);
        else sharing_mode = SharingMode::eExclusive;

        swap_chain_ = swap_chain_type{swap_chain_info_type{
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
        }};
    }

    void vulkan_sample::generate_image_view_create_infos()
    {
        using image_view_type = decltype(image_views_)::value_type;

        auto&& images = device_->getSwapchainImagesKHR(*swap_chain_, device_.dispatch());

        image_views_.resize(images.size());
        std::transform(
            images.begin(),
            images.end(),
            image_views_.begin(),
            [this](decltype(*images.begin()) image)mutable
            {
                return image_view_type{{
                    {},
                    std::move(image),
                    ImageViewType::e2D,
                    swap_chain_.info().info.imageFormat,
                    ComponentMapping{},
                    ImageSubresourceRange{ImageAspectFlagBits::eColor, 0, 1, 0, 1}
                    }};
            }
        );
    }

    void vulkan_sample::generate_render_pass_create_info()
    {
        using render_pass_type = decltype(render_pass_);
        using render_pass_info_type = decltype(render_pass_)::info_type;

        //the first subpass deal with the color attachment
        AttachmentDescription color_attachment = {
            {},
            swap_chain_.info().info.imageFormat,
            SampleCountFlagBits::e1,
            AttachmentLoadOp::eClear,
            AttachmentStoreOp::eStore,
            AttachmentLoadOp::eDontCare,
            AttachmentStoreOp::eDontCare,
            ImageLayout::eUndefined,
            ImageLayout::ePresentSrcKHR
        };
        AttachmentReference color_attachment_ref = {0, ImageLayout::eColorAttachmentOptimal};
        auto color_attachment_subpass = info_proxy<SubpassDescription>{
            {},
            vector<AttachmentReference>{std::move(color_attachment_ref)}
        };
        SubpassDependency dependency = {
            VK_SUBPASS_EXTERNAL,
            0,
            PipelineStageFlagBits::eColorAttachmentOutput,
            PipelineStageFlagBits::eColorAttachmentOutput,
            AccessFlags{},
            AccessFlagBits::eColorAttachmentRead | AccessFlagBits::eColorAttachmentWrite,
        };

        render_pass_ = render_pass_type{render_pass_info_type{
            {std::move(color_attachment)}, {std::move(color_attachment_subpass)}, {std::move(dependency)}}
        };
    }

    void vulkan_sample::generate_framebuffer_create_infos()
    {
        using frame_buffer_type = decltype(frame_buffers_)::value_type;
        using frame_buffer_info_type = frame_buffer_type::info_type;

        frame_buffers_.resize(image_views_.size());
        std::transform(
            image_views_.cbegin(),
            image_views_.cend(),
            frame_buffers_.begin(),
            [this](const image_view_object& image_view)
            {
                return frame_buffer_type{frame_buffer_info_type{
                    {*image_view},
                    frame_buffer_info_type::base_info_type{
                        {},
                        *render_pass_,
                        0,
                        nullptr,
                        swap_chain_.info().info.imageExtent.width,
                        swap_chain_.info().info.imageExtent.height,
                        1
                }
                }};
            }
        );
    }

    void vulkan_sample::generate_shader_module_create_infos()
    {
        using shader_module_type = decltype(vertex_shader_module_);
        using shader_module_info_type = shader_module_type::info_type;

        CompileOptions options;
        const auto shaders_path = path{"resource"} / "shaders";
        const auto vertex_shader_code_path = shaders_path / "shader.vert";
        const auto fragment_shader_code_path = shaders_path / "shader.frag";

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
                throw std::runtime_error("vertex code compile failure\n" + error_str);
            vertex_shader_module_ = shader_module_type{
                shader_module_info_type{std::move(spriv_code), {}}
            };
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
                throw std::runtime_error("fragment code compile failure\n" + error_str);
            fragment_shader_module_ = shader_module_type{
                shader_module_info_type{std::move(spriv_code)}
            };
        }
        cfin.close();
    }

    void vulkan_sample::generate_descriptor_set_layout_create_info()
    {
        using descriptor_set_layout_type = decltype(descriptor_set_layout_);
        using descriptor_set_layout_info_type = descriptor_set_layout_type::info_type;

        descriptor_set_layout_ = descriptor_set_layout_type{descriptor_set_layout_info_type{
            {DescriptorSetLayoutBinding{0, DescriptorType::eUniformBuffer, 1, ShaderStageFlagBits::eVertex}}
        }};
    }

    void vulkan_sample::generate_descriptor_pool_create_info()
    {
        using descriptor_pool_type = decltype(descriptor_pool_);
        using descriptor_pool_info_type = descriptor_pool_type::info_type;

        descriptor_pool_ = descriptor_pool_type{descriptor_pool_info_type{{{
                DescriptorType::eUniformBuffer,
                static_cast<uint32_t>(image_views_.size())
            }}, {DescriptorPoolCreateFlagBits::eFreeDescriptorSet}}};
    }

    void vulkan_sample::generate_descriptor_set_allocate_info()
    {
        using descriptor_set_type = decltype(descriptor_sets_)::value_type;
        using descriptor_set_info_type = descriptor_set_type::info_type;

        descriptor_sets_.resize(image_views_.size());
        for(auto& descriptor_set : descriptor_sets_)
            descriptor_set = descriptor_set_type{descriptor_set_info_type{
                {image_views_.size(), *descriptor_set_layout_},
                descriptor_set_info_type::base_info_type{*descriptor_pool_}
        }};
    }

    void vulkan_sample::generate_pipeline_layout_create_info()
    {
        using pipeline_layout_type = decltype(pipeline_layout_);
        using pipeline_layout_info_type = pipeline_layout_type::info_type;

        pipeline_layout_ = pipeline_layout_type{pipeline_layout_info_type{{*descriptor_set_layout_}}};
    }

    void vulkan_sample::generate_graphics_pipeline_create_info()
    {
        using graphics_pipeline_type = decltype(graphics_pipeline_);
        using graphics_pipeline_info_type = graphics_pipeline_type::info_type;

        GraphicsPipelineCreateInfo info;
        info.renderPass = *render_pass_;
        info.layout = *pipeline_layout_;

        auto vertex_shader_stage = info_proxy<PipelineShaderStageCreateInfo>{
            "main",
            nullopt,
            PipelineShaderStageCreateInfo{{}, ShaderStageFlagBits::eVertex, *vertex_shader_module_}
        };
        auto fragment_shader_stage = info_proxy<PipelineShaderStageCreateInfo>{
            "main",
            nullopt,
            PipelineShaderStageCreateInfo{{}, ShaderStageFlagBits::eFragment, *fragment_shader_module_}
        };
        auto input_state = info_proxy<PipelineVertexInputStateCreateInfo>{
            {vertex::description},
            {
                vertex::attribute_descriptions.cbegin(),
                vertex::attribute_descriptions.cend()
            }
        };
        PipelineInputAssemblyStateCreateInfo input_assembly_state = {{}, PrimitiveTopology::eTriangleList};
        auto viewport_state = info_proxy<PipelineViewportStateCreateInfo>{
            {
                {
                    0,
                    0,
                    static_cast<float>(swap_chain_.info().info.imageExtent.width),
                    static_cast<float>(swap_chain_.info().info.imageExtent.height),
                    0,
                    1
                }
            },
            vector<Rect2D>{Rect2D{{0, 0}, swap_chain_.info().info.imageExtent}}
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
                    ColorComponentFlagBits::eA | ColorComponentFlagBits::eR |
                    ColorComponentFlagBits::eG | ColorComponentFlagBits::eB
                }
            },
            {
                {},
                false,
                LogicOp::eCopy,
                0,
                nullptr,
                std::array<float, 4>{0}
            }
        };
        PipelineMultisampleStateCreateInfo multi_sample_state;

        graphics_pipeline_ = graphics_pipeline_type{info_proxy<graphics_pipeline_create_info>{
            { std::move(vertex_shader_stage), std::move(fragment_shader_stage) },
            {std::move(input_state)},
            {std::move(input_assembly_state)},
            {nullopt},
            {std::move(viewport_state)},
            {std::move(rasterization_state)},
            {std::move(multi_sample_state)},
            {nullopt},
            {std::move(color_blend_state)},
            {nullopt},
            {std::move(info)}
        }};
    }

    void vulkan_sample::generate_transform_buffer_create_info()
    {
        using uniform_buffer_type = decltype(transform_buffer_);
        using uniform_buffer_info_type = uniform_buffer_type::info_type;

        transform_buffer_ = uniform_buffer_type{
            uniform_buffer_info_type{{}, {{}, sizeof(transform), BufferUsageFlagBits::eUniformBuffer}}
        };
    }

    void vulkan_sample::generate_buffer_allocate_info()
    {
        transfer_memory_ = {*physical_device_, device_, {
            BufferUsageFlagBits::eVertexBuffer,
            BufferUsageFlagBits::eIndexBuffer
        }};
    }

    void vulkan_sample::generate_graphics_command_pool_create_info()
    {
        using command_pool_type = decltype(graphics_command_pool_);
        using command_pool_info_type = command_pool_type::info_type;

        graphics_command_pool_ = command_pool_type{
            command_pool_info_type{CommandPoolCreateFlagBits::eResetCommandBuffer, graphics_queue_index_}
        };
    }

    void vulkan_sample::generate_graphics_command_buffer_allocate_info()
    {
        using command_buffer_type = decltype(graphics_command_buffers_)::value_type;
        using command_buffer_info_type = command_buffer_type::info_type;

        graphics_command_buffers_.resize(frame_buffers_.size());
        for(auto& buffer : graphics_command_buffers_)
            buffer = command_buffer_type{{
                    *graphics_command_pool_,
                    CommandBufferLevel::ePrimary,
                    static_cast<decltype(buffer.info().commandBufferCount)>(frame_buffers_.size())
                }};
    }

    void vulkan_sample::generate_image_create_info()
    {
        using image_type = decltype(image_);
        using image_info_type = image_type::info_type;

        texture_image_ = stb::image<channel::rgb_alpha>{"freshman.jpg"};
        image_buffer_ = buffer_object{
            BufferCreateInfo{{},
            texture_image_.pixel_size() * sizeof(decltype(texture_image_)::pixel_t),
            BufferUsageFlagBits::eTransferSrc}
        };

        image_ = image_type{image_info_type::base_info_type{
            {},
            ImageType::e2D,
            Format::eR8G8B8A8Unorm,
            Extent3D{
            static_cast<uint32_t>(texture_image_.width()),
            static_cast<uint32_t>(texture_image_.height()),
            1
        },
            1,
            1,
            SampleCountFlagBits::e1,
            ImageTiling::eOptimal,
            ImageUsageFlagBits::eTransferDst | ImageUsageFlagBits::eSampled,
        }};
    }

    void vulkan_sample::generate_sync_objects_create_info()
    {
        using semaphore_type = decltype(swap_chain_image_syn_)::value_type;
        using semaphore_info_type = semaphore_type::info_type;

        using fence_type = decltype(gpu_syn_)::value_type;
        using fence_info_type = fence_type::info_type;

        swap_chain_image_syn_.resize(image_views_.size());
        render_syn_.resize(image_views_.size());
        gpu_syn_.resize(image_views_.size());

        for(auto& syn : gpu_syn_)
            syn = fence_type{fence_info_type{FenceCreateFlagBits::eSignaled}};
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

        transition_image_layout(
            front_command_buffer,
            *image_,
            ImageLayout::eUndefined,
            ImageLayout::eTransferDstOptimal,
            device_.dispatch()
        );
        front_command_buffer.copyBufferToImage(
            *image_buffer_,
            *image_,
            ImageLayout::eTransferDstOptimal,
            {BufferImageCopy{
                0, 0, 0, {ImageAspectFlagBits::eColor, 0, 0, 1}, {0, 0, 0}, image_.info().info.extent
            }},
            device_.dispatch()
        );
        transition_image_layout(
            front_command_buffer,
            *image_,
            ImageLayout::eTransferDstOptimal,
            ImageLayout::eShaderReadOnlyOptimal,
            device_.dispatch()
        );

        front_command_buffer.end(device_.dispatch());

        graphics_queue_.submit({front_submit_info}, nullptr, device_.dispatch());
    }

    void vulkan_sample::generate_render_info()
    {
        render_pass_begin_infos_.resize(frame_buffers_.size());
        submit_infos_.resize(graphics_command_buffers_.size());
        present_infos_.resize(submit_infos_.size());

        for(const auto& descriptor_set : descriptor_sets_)
            device_->updateDescriptorSets({info_proxy<WriteDescriptorSet>{
                {},
                {{*transform_buffer_, 0, whole_size<decltype(DescriptorBufferInfo::range)>}},
                {},
                {*descriptor_set, 0, 0, 1, DescriptorType::eUniformBuffer}
        }}, {}, device_.dispatch());

        submit_precondition_command();

        command_buffer_begin_info_ = CommandBufferBeginInfo{CommandBufferUsageFlagBits::eSimultaneousUse};
        graphics_queue_.waitIdle(device_.dispatch());
        std::for_each(
            graphics_command_buffers_.begin(),
            graphics_command_buffers_.end(),
            [this, index = size_t{0}](command_buffer_object& buffer)mutable
        {
            render_pass_begin_infos_[index] = info_proxy<RenderPassBeginInfo>{
                {{std::array<float, 4>{1, 1, 1, 1}}},
                RenderPassBeginInfo{
                RenderPass{*render_pass_},
                Framebuffer{*frame_buffers_[index]},
                Rect2D{{0, 0}, {swap_chain_.info().info.imageExtent}}
            }
            };

            buffer->begin(command_buffer_begin_info_, device_.dispatch());
            buffer->beginRenderPass(render_pass_begin_infos_[index], SubpassContents::eInline, device_.dispatch());
            buffer->bindPipeline(PipelineBindPoint::eGraphics, *graphics_pipeline_, device_.dispatch());
            buffer->bindVertexBuffers(0, {*transfer_memory_.device_local_buffer(vertices_buffer_index)}, {0}, device_.dispatch());
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
                vector<SwapchainKHR>{*swap_chain_},
                vector<uint32_t>{static_cast<uint32_t>(index)}
            };

            index++;
        }
        );
    }

    VkBool32 vulkan_sample::debug_callback(
        const VkDebugUtilsMessageSeverityFlagBitsEXT flag_bits,
        const VkDebugUtilsMessageTypeFlagsEXT type_flags,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
        void*)
    {
        const auto flag = DebugUtilsMessageSeverityFlagBitsEXT{flag_bits};
        std::cerr << to_string(flag) << '\n' <<
            to_string(DebugUtilsMessageTypeFlagsEXT{type_flags}) << '\n' <<
            "validation layer: " << p_callback_data->pMessage << '\n';

        return flag == DebugUtilsMessageSeverityFlagBitsEXT::eError ? true : false;
    }

    void vulkan_sample::initialize_vulkan()
    {
        generate_instance_create_info();
        instance_.initialize();
        if constexpr(is_debug)
            debug_messenger_.initialize(instance_);

        generate_surface_create_info();
        surface_.initialize(instance_);

        physical_device_ = {*instance_, [this](const auto& d) { return generate_physical_device(d); }};

        generate_device_create_info();
        device_.initialize(*physical_device_, instance_.dispatch());

        graphics_queue_ = device_->getQueue(graphics_queue_index_, 0, device_.dispatch());
        present_queue_ = device_->getQueue(present_queue_index_, 0, device_.dispatch());

        generate_swap_chain_create_info();
        swap_chain_.initialize(device_);

        generate_image_view_create_infos();
        for(auto& view : image_views_)
            view.initialize(device_);

        generate_render_pass_create_info();
        render_pass_.initialize(device_);

        generate_framebuffer_create_infos();
        for(auto& fb : frame_buffers_)
            fb.initialize(device_);

        generate_shader_module_create_infos();
        vertex_shader_module_.initialize(device_);
        fragment_shader_module_.initialize(device_);

        generate_descriptor_set_layout_create_info();
        descriptor_set_layout_.initialize(device_);

        generate_descriptor_pool_create_info();
        descriptor_pool_.initialize(device_);

        generate_descriptor_set_allocate_info();
        descriptor_sets_ = descriptor_pool_.create_element_objects(
            device_,
            descriptor_sets_.front().info().info
        );

        generate_pipeline_layout_create_info();
        pipeline_layout_.initialize(device_);

        generate_graphics_pipeline_create_info();
        graphics_pipeline_.initialize(device_);

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

        generate_graphics_command_pool_create_info();
        graphics_command_pool_.initialize(device_);

        generate_graphics_command_buffer_allocate_info();
        graphics_command_buffers_ = graphics_command_pool_.create_element_objects(
            device_,
            graphics_command_buffers_.front().info()
        );

        generate_buffer_allocate_info();
        transfer_memory_.initialize(*physical_device_);
        set_vertices({
            vertex{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            vertex{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            vertex{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            vertex{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
            });
        set_indices({0, 1, 2, 2, 3, 0});

        generate_image_create_info();
        image_.initialize(device_);
        image_buffer_.initialize(device_);
        image_buffer_memory_ = generate_buffer_memory_info(
            device_,
            {*image_buffer_},
            *physical_device_,
            MemoryPropertyFlagBits::eHostVisible
        ).first;
        image_buffer_memory_.initialize(device_);
        device_->bindBufferMemory(*image_buffer_, *image_buffer_memory_, 0, device_.dispatch());
        image_memory_ = generate_image_memory_info(
            device_,
            {*image_},
            *physical_device_,
            MemoryPropertyFlagBits::eDeviceLocal
        ).first;
        image_memory_.initialize(device_);
        device_->bindImageMemory(*image_, *image_memory_, 0, device_.dispatch());

        generate_sync_objects_create_info();
        for(auto& semaphore : swap_chain_image_syn_)
            semaphore.initialize(device_);
        for(auto& semaphore : render_syn_)
            semaphore.initialize(device_);
        for(auto& fence : gpu_syn_)
            fence.initialize(device_);

        generate_render_info();

        flush_to_memory();
    }

    void vulkan_sample::re_initialize_vulkan()
    {
        const auto old_extent = swap_chain_.info().info.imageExtent;
        const auto old_image_view_size = image_views_.size();
        const auto old_image_format = swap_chain_.info().info.imageFormat;
        {
            int window_width = 0, window_height = 0;
            while((window_width | window_height) == 0)
            {
                glfwGetFramebufferSize(window_, &window_width, &window_height);
                glfwWaitEvents();
            }
        }

        device_->waitIdle(device_.dispatch());

        for(auto& frame_buffer : frame_buffers_)
            frame_buffer = nullptr;

        for(auto& view : image_views_)
            view = nullptr;

        swap_chain_ = nullptr;

        generate_swap_chain_create_info();
        swap_chain_.initialize(device_);

        generate_image_view_create_infos();
        for(auto& view : image_views_)
            view.initialize(device_);

        if(swap_chain_.info().info.imageFormat != old_image_format)
        {
            generate_render_pass_create_info();
            render_pass_.initialize(device_);
        }

        generate_framebuffer_create_infos();
        for(auto& frame_buffer : frame_buffers_)
            frame_buffer.initialize(device_);

        if(image_views_.size() != old_image_view_size)
        {
            generate_descriptor_pool_create_info();
            descriptor_pool_.initialize(device_);

            generate_descriptor_set_allocate_info();
            descriptor_sets_ = descriptor_pool_.create_element_objects(
                device_,
                descriptor_sets_.front().info().info
            );

            generate_pipeline_layout_create_info();
            pipeline_layout_.initialize(device_);

            generate_graphics_command_buffer_allocate_info();
            graphics_command_buffers_ = graphics_command_pool_.create_element_objects(
                device_,
                graphics_command_buffers_
                .front().info()
            );
        }

        if(swap_chain_.info().info.imageExtent != old_extent)
        {
            generate_graphics_pipeline_create_info();
            graphics_pipeline_.initialize(device_);
        }

        generate_render_info();
    }

    void vulkan_sample::glfw_cleanup() noexcept
    {
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }

    vulkan_sample::~vulkan_sample()
    {
        glfw_cleanup();
    }

    auto vulkan_sample::fps() const ->  decltype(fps_) { return fps_; }

    const SwapchainCreateInfoKHR& vulkan_sample::swap_chain_create_info() const { return swap_chain_.info(); }

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
        device_->flushMappedMemoryRanges(
            {{*transform_buffer_memory_, 0, whole_size<DeviceSize>}},
            device_.dispatch()
        );
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

    void vulkan_sample::set_image(decltype(texture_image_) image)
    {
        texture_image_ = std::move(image);
        write(image_buffer_memory_, device_, texture_image_);
    }
}