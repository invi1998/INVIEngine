#pragma once

#include "EngineMinimal.h"
#include "Mesh/Core/Mesh.h"

// 圆柱、圆台
class CCylinderMesh : public CMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~CCylinderMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InTopRadius,				// 圆柱顶部半径
		float InBottomRadius,			// 圆柱底部半径
		float InHeight,					// 高度
		uint32_t InAxialSubdivision,	// 轴向细分
		uint32_t InHeightSubdivision	// 高度细分
	);
};

