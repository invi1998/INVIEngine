#include "EngineMinimal.h"

#include "PlaneMesh.h"

#include "Component/Mesh/PlaneMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"

GPlaneMesh::GPlaneMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = GetRootComponent();

	GMesh::SetMeshComponent(ConstructionObject<CPlaneMeshComponent>(params));
}

void GPlaneMesh::Init()
{
	GMesh::Init();
}

void GPlaneMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	CREATE_RENDER_DATA(CPlaneMeshComponent, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}
