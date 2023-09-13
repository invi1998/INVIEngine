#include "PlaneMesh.h"

#include "Core/MeshManager.h"

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
	SetMeshComponent(GetMeshManage()->CreatePlaneMeshComponent(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide));
}
