#include "SpotLightComponent.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/Core/MeshManager.h"

CSpotLightComponent::CSpotLightComponent(): CLightComponent()
{
	// ��ȡ���ģ��
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent("Asserts/Mesh/SpotMesh.obj"));

	// ���þ۹�ģ��Ϊ�߿�ģʽ��ʾ
	if (GetLightMesh())
	{
		if (CMaterial* material = (*GetLightMesh()->GetMaterial())[0])
		{
			material->SetMaterialType(EMaterialType::BaseColor);
			material->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(Colors::WhiteSmoke));
		}
	}

	LightType = ELightType::PointLight;
}

CSpotLightComponent::~CSpotLightComponent()
{
}

void CSpotLightComponent::SetStartAttenuation(float Start)
{
	StartAttenuation = Start;
}

void CSpotLightComponent::SetEndAttenuation(float end)
{
	EndAttenuation = end;
}

void CSpotLightComponent::SetKc(float c)
{
	Kc = c;
}

void CSpotLightComponent::SetKl(float l)
{
	Kl = l;
}

void CSpotLightComponent::SetKq(float q)
{
	Kq = q;
}
