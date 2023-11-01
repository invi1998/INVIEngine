#pragma once

#include "Core/ShellMeshComponent.h"

class CCylinderMeshComponent : public CShellMeshComponent
{
public:
	CCylinderMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InTopRadius,				// 圆柱顶部半径
		float InBottomRadius,			// 圆柱底部半径
		float InHeight,					// 高度
		uint32_t InAxialSubdivision,	// 轴向细分
		uint32_t InHeightSubdivision	// 高度细分
	);

	void BuildKey(size_t& meshKey,
		float InTopRadius,				// 圆柱顶部半径
		float InBottomRadius,			// 圆柱底部半径
		float InHeight,					// 高度
		uint32_t InAxialSubdivision,	// 轴向细分
		uint32_t InHeightSubdivision	// 高度细分
	);
};

