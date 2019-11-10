#pragma once

#include "vulkan/utility/utility_core.h"

namespace vulkan::utility::constant
{
	using namespace vk;

	using glm::float32;
	using glm::float64;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::uvec4;
	using glm::ivec2;

	template<typename T>
	constexpr auto whole_size = T{VK_WHOLE_SIZE};

	template<typename T>
	constexpr auto format = Format::eUndefined;

	template<>
	constexpr auto format<float32> = Format::eR32Sfloat;

	template<>
	constexpr auto format<float64> = Format::eR64Sfloat;

	template<>
	constexpr auto format<vec2> = Format::eR32G32Sfloat;

	template<>
	constexpr auto format<ivec2> = Format::eR32G32Sint;

	template<>
	constexpr auto format<vec3> = Format::eR32G32B32Sfloat;

	template<>
	constexpr auto format<vec4> = Format::eR32G32B32A32Sfloat;

	template<>
	constexpr auto format<uvec4> = Format::eR64Sfloat;;

	template<typename T>
	constexpr auto index_type = IndexType::eNoneNV;

	template<>
	constexpr auto index_type<uint32_t> = IndexType::eUint32;

	template<>
	constexpr auto index_type<uint16_t> = IndexType::eUint16;

	template<>
	constexpr auto index_type<uint8_t> = IndexType::eUint8EXT;
}
