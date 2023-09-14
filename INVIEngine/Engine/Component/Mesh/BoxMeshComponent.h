#pragma once

#include "EngineMinimal.h"
#include "ShellMeshComponent.h"

class CBoxMeshComponent : public CShellMeshComponent
{
public:
	CBoxMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);
};
