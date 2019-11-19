#include "image.h"

namespace vulkan::utility
{
    void transition_image_layout(
        const CommandBuffer command_buffer,
        const Image image,
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
                ImageSubresourceRange{ImageAspectFlagBits::eColor, 0, 1, 0, 1}
            }}, dispatch);
    }
}