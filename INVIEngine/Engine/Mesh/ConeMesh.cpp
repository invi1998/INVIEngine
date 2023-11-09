#include "EngineMinimal.h"

#include "ConeMesh.h"

#include "Component/Mesh/ConeMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"


GConeMesh::GConeMesh()
{
	GMesh::SetMeshComponent(ConstructionObject<CConeMeshComponent>());
}

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
	CREATE_RENDER_DATA(CConeMeshComponent, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

