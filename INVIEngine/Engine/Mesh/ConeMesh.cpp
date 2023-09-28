#include "EngineMinimal.h"

#include "ConeMesh.h"

#include "Core/MeshManager.h"


void GConeMesh::Init()
{
	GMesh::Init();
}


void GConeMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GConeMesh::CreateMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	SetMeshComponent(GetMeshManage()->CreateConeMeshComponent(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision));
}

