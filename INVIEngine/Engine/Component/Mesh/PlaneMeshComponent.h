#pragma once
#include "EngineMinimal.h"
#include "ShellMeshComponent.h"

class CPlaneMeshComponent : public CShellMeshComponent
{
public:
	CPlaneMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};
