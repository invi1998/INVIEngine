#pragma once
#include "EngineMinimal.h"
#include "Math/EngineMath.h"

// 顶点数据
struct FVertex
{
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor);
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor, const DirectX::XMFLOAT3& InNormal);

	DirectX::XMFLOAT3 Position;		// 顶点位置
	DirectX::XMFLOAT4 Color;		// 顶点颜色
	DirectX::XMFLOAT3 Normal;		// 顶点法线
};

struct FMeshRenderingData
{
	FMeshRenderingData() = default;
	FMeshRenderingData(const std::vector<uint16_t>& Index, const std::initializer_list<FVertex>& Vertex);

	UINT GetVertexSizeInBytes() const { return VertexData.size() * sizeof(FVertex); }
	UINT GetIndexSizeInBytes() const { return IndexData.size() * sizeof(uint16_t); }

	std::vector<FVertex> VertexData;	// 顶点数据
	std::vector<uint16_t> IndexData;	// 索引数据
};


