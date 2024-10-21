#include "EngineMinimal.h"
#include "PyramidMesh.h"
#include "Core/MeshType.h"
#include "PlaneMesh.h"
#include "Component/Mesh/PyramidMeshComponent.h"
#include "Core/MeshManager.h"
#include "Core/Construction/MacroConstruction.h"


GPyramidMesh::GPyramidMesh()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = GetRootComponent();

	GMesh::SetMeshComponent(ConstructionObject<CPyramidMeshComponent>(params));
}

void GPyramidMesh::Init()
{
	Super::Init();
}

void GPyramidMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void GPyramidMesh::CreateMesh(EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
	CREATE_RENDER_DATA(CPyramidMeshComponent, InPyramidNumberSidesType, InHeightSubdivide, InSize);
}
