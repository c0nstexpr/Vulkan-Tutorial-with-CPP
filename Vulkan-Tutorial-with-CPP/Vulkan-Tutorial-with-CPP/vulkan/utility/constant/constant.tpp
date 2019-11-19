#pragma once

namespace vulkan::utility::constant
{
	static constexpr pair<AccessFlagBits, PipelineStageFlagBits> required_access_and_pipeline_stage(const ImageLayout layout)
	{
		switch(layout)
		{
		case ImageLayout::eUndefined:
		return {{}, PipelineStageFlagBits::eTopOfPipe};

		case ImageLayout::eTransferDstOptimal:
		return  {AccessFlagBits::eTransferWrite, PipelineStageFlagBits::eTransfer};

		case ImageLayout::eShaderReadOnlyOptimal:
		return  {AccessFlagBits::eShaderRead, PipelineStageFlagBits::eFragmentShader};

		default: throw std::invalid_argument{"unsupported layout"};
		}
	}
}