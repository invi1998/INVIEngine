#include "EngineMinimal.h"

#include "SphereMeshComponent.h"

#include "Mesh/Core/MeshType.h"

CSphereMeshComponent::CSphereMeshComponent()
{
}

void CSphereMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	float Theta = XM_2PI / static_cast<float>(InHeightSubdivision);
	float Beta = XM_PI / static_cast<float>(InAxialSubdivision);

	// ����
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(Colors::White), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT2(1.f, 0.5f)));

	for (size_t i = 1; i < InAxialSubdivision; ++i)
	{
		float BetaValue = i * Beta;

		for (size_t j = 0; j <= InHeightSubdivision; ++j)
		{
			float ThetaValue = j * Theta;

			// �����ϸö�������
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(BetaValue) * cosf(ThetaValue),
					InRadius * cosf(BetaValue),
					InRadius * sinf(BetaValue) * sinf(ThetaValue)
				),
				XMFLOAT4(Colors::White)
			));

			int TopIndex = MeshData.VertexData.size() - 1;
			FVertex& InVertex = MeshData.VertexData[TopIndex];
			// �������ϸõ�ķ���
			XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);
			XMStoreFloat3(&InVertex.Normal, XMVector3Normalize(Pos));

			// U���������
			InVertex.UTangent.x = -InRadius * sinf(BetaValue) * sinf(ThetaValue);
			InVertex.UTangent.y = 0.f;
			InVertex.UTangent.z = InRadius * sinf(BetaValue) * cosf(ThetaValue);

			// uv
			InVertex.TexCoord.x = ThetaValue / XM_2PI;
			InVertex.TexCoord.y = BetaValue / XM_PI;

			// ��������ߺ󣬶�����ֵ���й�һ���洢
			XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));
		}
	}

	// �ײ�
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT2(0.f, 0.5f)));


	// ������������ (���Ʊ�����

	for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
	{
		// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	uint32_t BaseIndex = 1;
	// �����м�����ĵ�
	uint32_t VertexCircleNum = InAxialSubdivision + 1;
	for (uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			// ��������ϱ����м���Ƶ������ı���(��һ���ı���������������������ɵ�
			// ������1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			// ������2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	// �����ϼ�
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;		// ��������
	BaseIndex = SouthBaseIndex - VertexCircleNum;			// ��ƽ����Ƶ����ʼ�����������ȥ����������
	for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
	{
		// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + i);
		MeshData.IndexData.push_back(BaseIndex + i + 1);
	}
}

void CSphereMeshComponent::BuildKey(size_t& meshKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	constexpr std::hash<float> floatHash;

	meshKey = 8;
	meshKey += floatHash(InRadius);

	meshKey += std::hash<int>::_Do_hash(InAxialSubdivision);
	meshKey += std::hash<int>::_Do_hash(InHeightSubdivision);
}
