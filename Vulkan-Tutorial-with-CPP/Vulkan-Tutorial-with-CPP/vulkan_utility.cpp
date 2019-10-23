#include"vulkan_utility.h"

namespace vulkan
{
	namespace
	{
		Compiler compiler;
	}

	tuple<vector<AssemblyCompilationResult::element_type>, string, shaderc_compilation_status> glsl_compile_to_spriv_asm(
		const string& glsl_code,
		const shaderc_shader_kind kind,
		const char file_name[],
		const CompileOptions& options
	)
	{
		const auto& res = compiler.CompileGlslToSpvAssembly(glsl_code, kind, file_name, options);
		return {{res.cbegin(), res.cend()}, res.GetErrorMessage(), res.GetCompilationStatus()};
	}

	tuple<vector<SpvCompilationResult::element_type>, string, shaderc_compilation_status> glsl_compile_to_spriv(
		const string& glsl_code,
		const shaderc_shader_kind kind,
		const char file_name[],
		const CompileOptions& options
	)
	{
		const auto& res = compiler.CompileGlslToSpv(glsl_code, kind, file_name, options);
		return {{res.cbegin(), res.cend()}, res.GetErrorMessage(), res.GetCompilationStatus()};
	}

	const VertexInputBindingDescription vertex::description = {0, sizeof(vertex), VertexInputRate::eVertex};
	const array<VertexInputAttributeDescription, 2> vertex::attribute_descriptions = {
		VertexInputAttributeDescription{0, 0, format_v<vec2>, static_cast<uint32_t>(MEMBER_OFFSET(vertex, pos))},
		VertexInputAttributeDescription{1, 0, format_v<vec3>, static_cast<uint32_t>(MEMBER_OFFSET(vertex, color))}
	};
}