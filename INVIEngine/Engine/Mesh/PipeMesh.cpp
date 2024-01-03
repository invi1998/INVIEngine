#include "EngineMinimal.h"
#include "PipeMesh.h"

#include "Component/Mesh/PipeMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"

GPipeMesh::GPipeMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;

	GMesh::SetMeshComponent(ConstructionObject<CPipeMeshComponent>(params));
}

void GPipeMesh::Init()
{
	GMesh::Init();
}

void GPipeMesh::Draw(float DeltaTime)
{

}

void GPipeMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CPipeMeshComponent, InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision);
}
