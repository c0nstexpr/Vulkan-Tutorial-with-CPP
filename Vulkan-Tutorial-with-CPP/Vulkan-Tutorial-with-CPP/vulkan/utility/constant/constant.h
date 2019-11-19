#pragma once

#include "vulkan/utility/utility_core.h"

namespace vulkan::utility::constant
{
	using namespace vk;

	using std::pair;
	using glm::float32;
	using glm::float64;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::uvec4;
	using glm::ivec2;

	template<typename T = DeviceSize>
	static constexpr auto whole_size = T{VK_WHOLE_SIZE};

	template<typename T = uint32_t>
	static constexpr auto queue_family_ignore = T{VK_QUEUE_FAMILY_IGNORED};

	template<typename T>
	static constexpr auto format = Format::eUndefined;

	template<>
	static constexpr auto format<float32> = Format::eR32Sfloat;

	template<>
	static constexpr auto format<float64> = Format::eR64Sfloat;

	template<>
	static constexpr auto format<vec2> = Format::eR32G32Sfloat;

	template<>
	static constexpr auto format<ivec2> = Format::eR32G32Sint;

	template<>
	static constexpr auto format<vec3> = Format::eR32G32B32Sfloat;

	template<>
	static constexpr auto format<vec4> = Format::eR32G32B32A32Sfloat;

	template<>
	static constexpr auto format<uvec4> = Format::eR64Sfloat;

	template<typename T>
	static constexpr auto index_type = IndexType::eNoneNV;

	template<>
	static constexpr auto index_type<uint32_t> = IndexType::eUint32;

	template<>
	static constexpr auto index_type<uint16_t> = IndexType::eUint16;

	template<>
	static constexpr auto index_type<uint8_t> = IndexType::eUint8EXT;

	static constexpr pair<AccessFlagBits, PipelineStageFlagBits> required_access_and_pipeline_stage(const ImageLayout);
}

#include "constant.tpp"