#include "EngineMinimal.h"
#include "PipeMesh.h"

#include "Core/MeshManager.h"

void GPipeMesh::Init()
{

}

void GPipeMesh::Draw(float DeltaTime)
{

}

void GPipeMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	SetMeshComponent(GetMeshManage()->CreatePipeMeshComponent(InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision));
}
