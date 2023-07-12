#pragma once
#include "EngineMinimal.h"
#include "Core/Mesh.h"

class CPlaneMesh : public CMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~CPlaneMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};

