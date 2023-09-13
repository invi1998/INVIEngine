#pragma once

#include "EngineMinimal.h"
#include "ShellMeshComponent.h"

class CConeMeshComponent : public CShellMeshComponent
{
public:
	CConeMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InBottomRadius,			// 圆柱底部半径
		float InHeight,					// 高度
		uint32_t InAxialSubdivision,	// 轴向细分
		uint32_t InHeightSubdivision	// 高度细分
	);
};

