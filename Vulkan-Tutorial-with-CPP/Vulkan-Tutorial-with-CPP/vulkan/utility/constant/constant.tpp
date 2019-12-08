#pragma once

namespace vulkan::utility::constant
{
	constexpr pair<AccessFlags, PipelineStageFlagBits> required_access_and_pipeline_stage(const ImageLayout layout)
	{
		switch(layout)
		{
		case ImageLayout::eUndefined:
		return {{}, PipelineStageFlagBits::eTopOfPipe};

		case ImageLayout::eTransferDstOptimal:
		return  {AccessFlagBits::eTransferWrite, PipelineStageFlagBits::eTransfer};

		case ImageLayout::eShaderReadOnlyOptimal:
		return  {AccessFlagBits::eShaderRead, PipelineStageFlagBits::eFragmentShader};

		case ImageLayout::eDepthStencilAttachmentOptimal:
		return {
			AccessFlagBits::eDepthStencilAttachmentRead | AccessFlagBits::eDepthStencilAttachmentWrite,
			PipelineStageFlagBits::eEarlyFragmentTests
		};

		default: throw std::invalid_argument{"unsupported layout"};
		}
	}
}