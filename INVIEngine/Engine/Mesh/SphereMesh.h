#pragma once
#include "Core/Mesh.h"

class GSphereMesh : public GMesh
{
public:
	~GSphereMesh() override;


	void BeginInit() override;
	void Tick(float DeltaTime) override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

};

