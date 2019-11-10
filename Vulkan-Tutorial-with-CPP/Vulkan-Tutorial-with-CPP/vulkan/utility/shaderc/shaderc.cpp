#include "shaderc.h"

namespace vulkan::utility::shaderc
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
}