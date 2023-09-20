#include "PointLightComponent.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/Core/MeshManager.h"

CPointLightComponent::CPointLightComponent()
	: CLightComponent()
{
	// 读取点光模型
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent("Asserts/Mesh/PointMesh.obj"));

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

void CPointLightComponent::SetStartAttenuation(float Start)
{
	StartAttenuation = Start;
}

void CPointLightComponent::SetEndAttenuation(float end)
{
	EndAttenuation = end;
}

void CPointLightComponent::SetKc(float c)
{
	Kc = c;
}

void CPointLightComponent::SetKl(float l)
{
	Kl = l;
}

void CPointLightComponent::SetKq(float q)
{
	Kq = q;
}

