#pragma once

#include "Math/EngineMath.h"

// ��������
struct FVertex
{
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor);
	FVertex(const DirectX::XMFLOAT3 &InPos, const DirectX::XMFLOAT4 &InColor, const DirectX::XMFLOAT3& InNormal, const XMFLOAT2& InTexcoord = XMFLOAT2(0.f, 0.f));

	DirectX::XMFLOAT3 Position;		// ����λ��
	DirectX::XMFLOAT4 Color;		// ������ɫ
	DirectX::XMFLOAT3 Normal;		// ���㷨��
	DirectX::XMFLOAT3 UTangent;		// �������ߣ�U���ߣ���ֱ������ߣ�
	// DirectX::XMFLOAT3 VTangent;		// �������ߣ�V���ߣ�ˮƽ������ߣ�
	XMFLOAT2 TexCoord;				// uv���꣨�������꣩
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

//Pyramid�ı���
enum EPyramidNumberSides
{
	Pyramid_3 = 3,
	Pyramid_4,
	Pyramid_5,
};

