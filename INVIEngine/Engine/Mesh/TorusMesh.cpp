#include "EngineMinimal.h"
#include "TorusMesh.h"

#include "Component/Mesh/TorusMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"

GTorusMesh::GTorusMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = GetRootComponent();

	GMesh::SetMeshComponent(ConstructionObject<CTorusMeshComponent>(params));
}

void GTorusMesh::Init()
{

}

void GTorusMesh::Draw(float DeltaTime)
{

}

void GTorusMesh::CreateMesh(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CTorusMeshComponent, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);
}
