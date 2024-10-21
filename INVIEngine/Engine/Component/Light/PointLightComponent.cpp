#include "EngineMinimal.h"

#include "PointLightComponent.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/Core/MeshManager.h"

CPointLightComponent::CPointLightComponent()
	: CRangeLightComponent()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = this;

	// 读取点光模型
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent(params, "Asserts/Mesh/PointMesh.obj"));

	// 设置点光模型为线框模式显示
	if (GetLightMesh())
	{
		if (CMaterial* material = (*GetLightMesh()->GetMaterial())[0])
		{
			material->SetMaterialType(EMaterialType::BaseColor);
			material->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(Colors::AntiqueWhite));
		}
	}

	LightType = ELightType::PointLight;
}

CPointLightComponent::~CPointLightComponent()
{
}

