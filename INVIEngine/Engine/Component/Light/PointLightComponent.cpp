#include "EngineMinimal.h"

#include "PointLightComponent.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"
#include "Mesh/Core/MeshManager.h"

CPointLightComponent::CPointLightComponent()
	: CRangeLightComponent()
{
	// ��ȡ���ģ��
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent("Asserts/Mesh/PointMesh.obj"));

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

	LightType = ELightType::PointLight;
}

CPointLightComponent::~CPointLightComponent()
{
}

