#include "EngineMinimal.h"

#include "CustomMesh.h"

#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"

GCustomMesh::GCustomMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;

	GMesh::SetMeshComponent(ConstructionObject<CCustomMeshComponent>(params));
}

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
	CREATE_RENDER_DATA(CCustomMeshComponent, InPath);
}

