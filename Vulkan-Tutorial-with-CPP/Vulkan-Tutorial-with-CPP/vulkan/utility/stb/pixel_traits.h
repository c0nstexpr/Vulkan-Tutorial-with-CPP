#pragma once

#include "utility/utility.h"

#define STBI_WINDOWS_UTF8
#include <stb_image.h>

namespace vulkan::utility::stb
{
	using glm::vec;
	using glm::length_t;

	enum class channel
	{
		default_desired = STBI_default,
		grey = STBI_grey,
		grey_alpha = STBI_grey_alpha,
		rgb = STBI_rgb,
		rgb_alpha = STBI_rgb_alpha
	};

	using channel_underlying_type = std::underlying_type_t<channel>;

	template<channel Channel>
	using pixel_t = vec<static_cast<length_t>(Channel), uint8_t>;
}
