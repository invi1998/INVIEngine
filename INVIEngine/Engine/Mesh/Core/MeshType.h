#pragma once
#include "EngineMinimal.h"
#include "Math/EngineMath.h"

// ��������
struct FVertex
{
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor);

	DirectX::XMFLOAT3 Position;		// ����λ��
	DirectX::XMFLOAT4 Color;		// ������ɫ
};

struct FMeshRendingData
{
	FMeshRendingData(const std::vector<uint16_t>& Index, const std::initializer_list<FVertex>& Vertex);

	std::vector<uint16_t> IndexData;	// ��������
	std::vector<FVertex> VertexData;	// ��������
};


