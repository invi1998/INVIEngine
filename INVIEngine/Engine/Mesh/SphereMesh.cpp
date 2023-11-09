#include "EngineMinimal.h"

#include "SphereMesh.h"

#include "Component/Mesh/SphereMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"


GSphereMesh::GSphereMesh()
{
	GMesh::SetMeshComponent(ConstructionObject<CSphereMeshComponent>());
}

void GSphereMesh::Init()
{
	GMesh::Init();
}

void GSphereMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GSphereMesh::CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{
	CREATE_RENDER_DATA(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision, bReverse);
}

