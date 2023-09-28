#include "EngineMinimal.h"

#include "CylinderMesh.h"

#include "Core/MeshManager.h"

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
	SetMeshComponent(GetMeshManage()->CreateCylinderMeshComponent(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision));
}

