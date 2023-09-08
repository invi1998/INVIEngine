#pragma once
#include "EngineMinimal.h"
#include "Math/EngineMath.h"

// ��������
struct FVertex
{
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor);
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor, const DirectX::XMFLOAT3& InNormal);

	DirectX::XMFLOAT3 Position;		// ����λ��
	DirectX::XMFLOAT4 Color;		// ������ɫ
	DirectX::XMFLOAT3 Normal;		// ���㷨��
};

struct FMeshRenderingData
{
	FMeshRenderingData() = default;
	FMeshRenderingData(const std::vector<uint16_t>& Index, const std::initializer_list<FVertex>& Vertex);

	UINT GetVertexSizeInBytes() const { return VertexData.size() * sizeof(FVertex); }
	UINT GetIndexSizeInBytes() const { return IndexData.size() * sizeof(uint16_t); }

	std::vector<FVertex> VertexData;	// ��������
	std::vector<uint16_t> IndexData;	// ��������
};


