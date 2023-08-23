#pragma once

#include "EngineMinimal.h"
#include "Core/Mesh.h"


// 自定义网格
class GCustomMesh : public GMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~GCustomMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void CreateMesh(FMeshRenderingData& MeshData, const std::string& InPath);

	static bool LoadObjFormBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData &MeshData);
};

