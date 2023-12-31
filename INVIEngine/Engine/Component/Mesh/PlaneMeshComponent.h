#pragma once

#include "Core/ShellMeshComponent.h"

class CPlaneMeshComponent : public CShellMeshComponent
{
public:
	CPlaneMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

	void BuildKey(size_t& meshKey, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};

