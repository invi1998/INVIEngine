#include "EngineMinimal.h"

#include "CylinderMesh.h"

#include "Component/Mesh/CylinderMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"

GCylinderMesh::GCylinderMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = GetRootComponent();

	GMesh::SetMeshComponent(ConstructionObject<CCylinderMeshComponent>(params));
}

void GCylinderMesh::Init()
{
	GMesh::Init();
}

void GCylinderMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GCylinderMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CCylinderMeshComponent, InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

