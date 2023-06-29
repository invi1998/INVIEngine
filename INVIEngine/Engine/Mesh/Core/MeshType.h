#pragma once
#include "EngineMinimal.h"
#include "Math/EngineMath.h"

// ��������
struct FVertex
{
	DirectX::XMFLOAT3 Position;		// ����λ��
	DirectX::XMFLOAT4 Color;		// ������ɫ
};

struct FMeshRendingData
{
	std::vector<FVertex> VertexData;	// ��������
	std::vector<uint16_t> IndexData;	// ��������
};


