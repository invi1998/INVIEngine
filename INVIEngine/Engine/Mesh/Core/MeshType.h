#pragma once
#include "EngineMinimal.h"
#include "Math/EngineMath.h"

// 顶点数据
struct FVertex
{
	DirectX::XMFLOAT3 Position;		// 顶点位置
	DirectX::XMFLOAT4 Color;		// 顶点颜色
};

struct FMeshRendingData
{
	std::vector<FVertex> VertexData;	// 顶点数据
	std::vector<uint16_t> IndexData;	// 索引数据
};


