#pragma once
#include <shaderc/shaderc.hpp>
#include "utility/utility.h"

namespace vulkan::utility::shaderc
{
	using namespace ::utility;
	using namespace ::shaderc;

	[[nodiscard]] inline tuple<vector<AssemblyCompilationResult::element_type>, string,
		shaderc_compilation_status> glsl_compile_to_spriv_asm(
			const string&,
			const shaderc_shader_kind,
			const char[],
			const CompileOptions & = {});

	[[nodiscard]] tuple<std::vector<SpvCompilationResult::element_type>, string, shaderc_compilation_status>
		glsl_compile_to_spriv(
			const string&,
			const shaderc_shader_kind,
			const char[],
			const CompileOptions & = {}
	);
}
