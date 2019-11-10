#pragma once

#include "utility/utility.h"

#define STBI_WINDOWS_UTF8
#include <stb_image.h>

namespace vulkan::utility::stb
{
	using namespace ::utility;

	enum class channel
	{
		default_desired = STBI_default,
		grey = STBI_grey,
		grey_alpha = STBI_grey_alpha,
		rgb = STBI_rgb,
		rgb_alpha = STBI_rgb_alpha
	};

	template<channel Channel>
	using pixel_t = vec<static_cast<length_t>(Channel), float>;
}
