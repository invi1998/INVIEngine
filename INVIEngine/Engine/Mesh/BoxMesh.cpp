#include "EngineMinimal.h"

#include "BoxMesh.h"

#include "Component/Mesh/BoxMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/ActorMeshConstruction.h"
#include "Core/Construction/MacroConstruction.h"

GBoxMesh::GBoxMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = GetRootComponent();

	GMesh::SetMeshComponent(ConstructionObject<CBoxMeshComponent>(params));
}

GBoxMesh::~GBoxMesh()
{
}

void GBoxMesh::Init()
{
	Super::Init();
}

void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void GBoxMesh::CreateMesh(float InHeight, float InWidth, float InDepth)
{
	CREATE_RENDER_DATA(CBoxMeshComponent, InHeight, InWidth, InDepth);
}
