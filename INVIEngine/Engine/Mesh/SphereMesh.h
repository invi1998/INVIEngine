#pragma once
#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
public:
	CSphereMesh();
	CSphereMesh(float radius, uint32_t axial, uint32_t height);
	~CSphereMesh() override;


	void BeginInit() override;
	void Tick(float DeltaTime) override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRendingData* InRenderingData) override;

	void BuildSphere();

private:
	float Radius;					// 半径
	uint32_t AxialSubdivision;			// 轴向细分（垂直 纬线）
	uint32_t HeightSubdivision;			// 高度细分（横向 经线）

	float Theta;
	float Beta;

	FMeshRendingData MeshData;

};

