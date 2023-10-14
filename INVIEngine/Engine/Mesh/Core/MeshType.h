#pragma once

#include "Math/EngineMath.h"

// 顶点数据
struct FVertex
{
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor);
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor, const DirectX::XMFLOAT3& InNormal, const XMFLOAT2& InTexcoord = XMFLOAT2(0.f, 0.f));

	DirectX::XMFLOAT3 Position;		// 顶点位置
	DirectX::XMFLOAT4 Color;		// 顶点颜色
	DirectX::XMFLOAT3 Normal;		// 顶点法线
	DirectX::XMFLOAT3 UTangent;		// 顶点切线（U切线，垂直向的切线）
	// DirectX::XMFLOAT3 VTangent;		// 顶点切线（V切线，水平向的切线）
	XMFLOAT2 TexCoord;				// uv坐标（纹理坐标）
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

//Pyramid的边数
enum EPyramidNumberSides
{
	Pyramid_3 = 3,
	Pyramid_4,
	Pyramid_5,
};

