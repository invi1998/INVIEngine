#pragma once

#include "Core/ShellMeshComponent.h"

// ��Դ����
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

	// OBJ ��ʽ
	static bool LoadObjFromBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& MeshData);
	// FBX ��ʽ
	static bool LoadFbxFromBuffer(const std::string& InPath, FMeshRenderingData& MeshData);

	void BuildKey(size_t& meshKey, const std::string& InPath);
};

