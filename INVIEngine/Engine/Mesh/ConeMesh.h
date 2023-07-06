#pragma once
#include "EngineMinimal.h"
#include "Core/Mesh.h"

// 圆锥
class CConeMesh : public CMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~CConeMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRendingData* InRenderingData) override;

	static CConeMesh* CreateMesh(
		float InBottomRadius,			// 圆柱底部半径
		float InHeight,					// 高度
		uint32_t InAxialSubdivision,	// 轴向细分
		uint32_t InHeightSubdivision	// 高度细分
	);
};

