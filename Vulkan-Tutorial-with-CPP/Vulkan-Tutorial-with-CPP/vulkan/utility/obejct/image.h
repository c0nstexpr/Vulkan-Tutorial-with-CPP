#pragma once
#include "vulkan/utility/utility_core.h"
#include "vulkan/utility/constant/constant.h"

namespace vulkan::utility
{
    void transition_image_layout(
        const CommandBuffer,
        const Image,
        const ImageLayout,
        const ImageLayout,
        const DispatchLoaderDynamic&
    );
}
