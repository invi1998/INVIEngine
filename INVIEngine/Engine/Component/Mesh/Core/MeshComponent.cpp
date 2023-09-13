#include "MeshComponent.h"

#include "Material/Core/Material.h"

CMeshComponent::CMeshComponent()
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
