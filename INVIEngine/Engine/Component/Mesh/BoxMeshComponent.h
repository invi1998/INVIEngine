#pragma once

#include "Core/ShellMeshComponent.h"

struct FMeshRenderingData;

class CBoxMeshComponent : public CShellMeshComponent
{
public:
	CBoxMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);
};

