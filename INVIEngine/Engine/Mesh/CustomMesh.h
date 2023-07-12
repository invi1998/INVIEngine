#pragma once

#include "EngineMinimal.h"
#include "Core/Mesh.h"


// 自定义网格
class CCustomMesh : public CMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~CCustomMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void CreateMesh(FMeshRenderingData& MeshData, const std::string& InPath);

	static bool LoadObjFormBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData &MeshData);
};

