#include "SphereMesh.h"

#include "Core/MeshManager.h"


void GSphereMesh::Init()
{
	GMesh::Init();
}

void GSphereMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GSphereMesh::CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	SetMeshComponent(GetMeshManage()->CreateSphereMeshComponent(InRadius, InAxialSubdivision, InHeightSubdivision));
}

