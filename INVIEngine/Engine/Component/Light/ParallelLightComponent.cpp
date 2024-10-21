#include "EngineMinimal.h"

#include "ParallelLightComponent.h"
#include "Core/LightComponent.h"
#include "Material/Core/Material.h"
#include "Mesh/Core/MeshManager.h"

CParallelLightComponent::CParallelLightComponent()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = this;

	// 读取平行光模型
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent(params, "Asserts/Mesh/SunMesh.obj"));

	// 设置平行光为线框模式显示
	if (GetLightMesh())
	{
		if (CMaterial* material = (*GetLightMesh()->GetMaterial())[0])
		{
			material->SetMaterialType(EMaterialType::BaseColor);
			material->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(Colors::AntiqueWhite));
		}
	}

	LightType = ELightType::DirectionalLight;
	
}

CParallelLightComponent::~CParallelLightComponent()
{
}

