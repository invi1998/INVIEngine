#include "CustomMesh.h"

#include "Core/MeshManager.h"

void GCustomMesh::Init()
{
	GMesh::Init();
}

void GCustomMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GCustomMesh::CreateMesh(const string& InPath)
{
	SetMeshComponent(GetMeshManage()->CreateMeshComponent(InPath));
}

