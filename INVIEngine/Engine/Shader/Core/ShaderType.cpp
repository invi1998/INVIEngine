#include "EngineMinimal.h"
#include "ShaderType.h"


bool ShaderType::ToD3DShaderMacro(const std::vector<FShaderMacro>& shaderMacro, std::vector<D3D_SHADER_MACRO>& d3dShaderMacro)
{
	for (auto& tem : shaderMacro)
	{
		D3D_SHADER_MACRO shaderM = {
			tem.Name.c_str(),
			tem.Definition.c_str()
		};

		d3dShaderMacro.push_back(shaderM);
	}

	if (d3dShaderMacro.size() > 0)
	{
		D3D_SHADER_MACRO macro{ NULL, NULL };
		d3dShaderMacro.push_back(macro);
	}

	return d3dShaderMacro.size();
}
