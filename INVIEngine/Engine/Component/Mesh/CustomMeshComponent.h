#pragma once

#include "Core/ShellMeshComponent.h"

// 资源类型
enum EMeshAssetsType
{
	OBJ,
	FBX,
};

class CCustomMeshComponent : public CShellMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, const std::string& InPath);

	// OBJ 格式
	static bool LoadObjFromBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& MeshData);
	// FBX 格式
	static bool LoadFbxFromBuffer(const std::string& InPath, FMeshRenderingData& MeshData);

	void BuildKey(size_t& meshKey, const std::string& InPath);
};

