#include "ParallelLightComponent.h"
#include "Core/LightComponent.h"
#include "Material/Core/Material.h"
#include "Mesh/Core/MeshManager.h"

CParallelLightComponent::CParallelLightComponent()
{
	ParallelLightMesh = GetMeshManage()->CreateMeshComponent("Asserts/Mesh/SunMesh.obj");

	if (ParallelLightMesh)
	{
		ParallelLightMesh->SetPosition({ 1.f, 1.f, 1.f });
		if (CMaterial* material = (*ParallelLightMesh->GetMaterial())[0])
		{
			material->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			material->SetBaseColor(XMFLOAT4(Colors::OrangeRed));
			material->SetMaterialType(EMaterialType::BaseColor);
		}
	}
	
	
}

CParallelLightComponent::~CParallelLightComponent()
{
}

void CParallelLightComponent::BeginInit()
{
	CLightComponent::BeginInit();
}

void CParallelLightComponent::Tick(float DeltaTime)
{
	CLightComponent::Tick(DeltaTime);
}
