#pragma once

namespace ShaderType
{
	struct FShaderMacro
	{
		std::string Name;
		std::string Definition;
	};

	bool ToD3DShaderMacro(const std::vector<FShaderMacro>& shaderMacro, std::vector<D3D_SHADER_MACRO>& d3dShaderMacro);
}


