#include "SpotLightComponent.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/Core/MeshManager.h"

CSpotLightComponent::CSpotLightComponent()
	: CLightComponent()
{
	// ��ȡ���ģ��
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent("Asserts/Mesh/SpotMesh.obj"));

	// ���õ��ģ��Ϊ�߿�ģʽ��ʾ
	if (GetLightMesh())
	{
		if (CMaterial* material = (*GetLightMesh()->GetMaterial())[0])
		{
			material->SetMaterialType(EMaterialType::BaseColor);
			material->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(Colors::AntiqueWhite));
		}
	}

	LightType = ELightType::SpotLight;
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

