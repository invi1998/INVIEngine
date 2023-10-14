#pragma once

#include "Core/ShellMeshComponent.h"

class CCustomMeshComponent : public CShellMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, const std::string& InPath);

	static bool LoadObjFormBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& MeshData);
};

