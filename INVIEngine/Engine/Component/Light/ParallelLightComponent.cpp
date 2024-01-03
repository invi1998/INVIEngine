#include "EngineMinimal.h"

#include "ParallelLightComponent.h"
#include "Core/LightComponent.h"
#include "Material/Core/Material.h"
#include "Mesh/Core/MeshManager.h"

CParallelLightComponent::CParallelLightComponent()
{
	FCreateObjectParams params{};
	params.Owner = this;

	// ��ȡƽ�й�ģ��
	SetLightMesh(GetMeshManage()->CreateCustomMeshComponent(params, "Asserts/Mesh/SunMesh.obj"));

	// ����ƽ�й�Ϊ�߿�ģʽ��ʾ
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

