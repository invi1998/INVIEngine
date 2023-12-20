#include "EngineMinimal.h"

#include "MeshComponent.h"

#include "Material/Core/Material.h"

CMeshComponent::CMeshComponent()
	: MeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_TRANSPARENT)
{
	Materials.push_back(CreateObject<CMaterial>(new CMaterial()));
}

CMeshComponent::~CMeshComponent()
{
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();
}

void CMeshComponent::BeginInit()
{
	CTransformationComponent::BeginInit();
}

void CMeshComponent::Tick(float DeltaTime)
{
	CTransformationComponent::Tick(DeltaTime);
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::Init()
{
}

void CMeshComponent::SetRenderLayerType(EMeshRenderLayerType type)
{
	MeshRenderLayerType = type;
}

bool CMeshComponent::IsDynamicReflection() const
{
	if (Materials.size() > 0)
	{
		return Materials[0]->IsDynamicReflection() && MeshRenderLayerType == EMeshRenderLayerType::RENDER_LAYER_OPAQUE_REFLECT;
	}

	return false;
}

bool CMeshComponent::IsCastShadow() const
{
	return CastShadow;
}
