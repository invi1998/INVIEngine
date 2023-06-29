#include "BoxMesh.h"

FBoxMesh::FBoxMesh()
{
}

FBoxMesh::~FBoxMesh()
{
}

void FBoxMesh::Init()
{
	Super::Init();
}

void FBoxMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}

void FBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

FBoxMesh* FBoxMesh::CreateMesh()
{
	FMeshRendingData MeshRenderingData;
	FBoxMesh* Box = new FBoxMesh();

	Box->BuildMesh(&MeshRenderingData);

	Box->Init();

	return Box;

}
