#include "EngineMinimal.h"

#include "SpotLightComponent.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/Core/MeshManager.h"

CSpotLightComponent::CSpotLightComponent(): CRangeLightComponent()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = this;

	// 读取点光模型
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent(params, "Asserts/Mesh/SpotMesh.obj"));

	// 设置聚光模型为线框模式显示
	if (GetLightMesh())
	{
		if (CMaterial* material = (*GetLightMesh()->GetMaterial())[0])
		{
			material->SetMaterialType(EMaterialType::BaseColor);
			material->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(Colors::WhiteSmoke));
		}
	}

	LightType = ELightType::SpotLight;
}

CSpotLightComponent::~CSpotLightComponent()
{
}

float CSpotLightComponent::GetSpotInnerCornerPhi() const
{
	return SpotInnerCornerPhi;
}

float CSpotLightComponent::GetSpotOuterCornerTheta() const
{
	return SpotOuterCornerTheta;
}

void CSpotLightComponent::SetSpotInnerCornerPhi(float phi)
{
	if (SpotOuterCornerTheta < phi)
	{
		SpotOuterCornerTheta = phi;
		SpotInnerCornerPhi = phi;
	}
	else if (SpotOuterCornerTheta > phi)
	{
		SpotInnerCornerPhi = phi;
	}
}

void CSpotLightComponent::SetSpotOuterCornerTheta(float theta)
{
	if (SpotInnerCornerPhi > theta)
	{
		SpotOuterCornerTheta = theta;
		SpotInnerCornerPhi = theta;
	}
	else if (SpotInnerCornerPhi < theta)
	{
		SpotOuterCornerTheta = theta;
	}
}

