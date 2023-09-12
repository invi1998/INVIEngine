#pragma once

#include "EngineMinimal.h"
#include "Core/MeshComponent.h"
#include "Mesh/Core/MeshType.h"

class CShellMeshComponent : public CMeshComponent
{
public:
	CShellMeshComponent();
	void BuildMesh(const FMeshRenderingData* mesh_rendering_data);
	void Init();
};

