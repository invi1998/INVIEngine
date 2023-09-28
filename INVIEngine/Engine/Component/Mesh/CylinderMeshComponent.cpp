#include "EngineMinimal.h"

#include "CylinderMeshComponent.h"

#include "Mesh/Core/MeshType.h"

CCylinderMeshComponent::CCylinderMeshComponent()
{
}

void CCylinderMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius,
	float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	// �뾶���
	float RadiusInterval = (InTopRadius - InBottomRadius) / static_cast<float>(InHeightSubdivision);

	// �߶ȼ��
	float HeightInterval = InHeight / static_cast<float>(InHeightSubdivision);

	// ����
	float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	// ��������
	for (uint32_t i = 0; i <= InHeightSubdivision; ++i)
	{
		float Y = (0.5f * InHeight) - HeightInterval * i;
		float Radius = InTopRadius + i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),	// x
					Y,							// Y
					Radius * sinf(j * BetaValue)	// z
				),
				XMFLOAT4(Colors::White)
			));
		}
	}

	// ����Բ����Χ index
	uint32_t VertexCircleNum = InAxialSubdivision + 1;
	for (uint32_t i = 0; i <= InHeightSubdivision; ++i)
	{
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			// �ϱ����м���Ƶ������ı���(��һ���ı���������������������ɵ�

			// ����Զ�������
			// ������1
			// MeshData.IndexData.push_back(i * VertexCircleNum + j);
			// MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j);
			// MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			// // ������2
			// MeshData.IndexData.push_back(i * VertexCircleNum + j);
			// MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			// MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);

			 // ���߳����Լ�
			 // ������1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(i * VertexCircleNum + j);
			// ������2
			MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(i * VertexCircleNum + j);
		}
	}

	// ��������
	{
		uint32_t Index = MeshData.VertexData.size();
		float Y = 0.5f * InHeight;
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * cosf(i * BetaValue),	// x
					Y,							// Y
					InTopRadius * sinf(i * BetaValue)	// z
				),
				XMFLOAT4(Colors::White)
			));
		}

		MeshData.VertexData.push_back(FVertex(
			XMFLOAT3(
				0.f,	// x
				Y,	// Y
				0.f	// z
			),
			XMFLOAT4(Colors::White)
		));

		// index
		float CenterPoint = MeshData.VertexData.size() - 1;
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
			MeshData.IndexData.push_back(CenterPoint);
			MeshData.IndexData.push_back(Index + i + 1);
			MeshData.IndexData.push_back(Index + i);
		}

	}

	// �����ײ�
	{
		uint32_t Index = MeshData.VertexData.size();
		float Y = -0.5f * InHeight;

		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * cosf(i * BetaValue),	// x
					Y,							// Y
					InTopRadius * sinf(i * BetaValue)	// z
				),
				XMFLOAT4(Colors::White)
			));
		}

		MeshData.VertexData.push_back(FVertex(
			XMFLOAT3(
				0.f,	// x
				Y,	// Y
				0.f	// z
			),
			XMFLOAT4(Colors::White)
		));

		// index
		float CenterPoint = MeshData.VertexData.size() - 1;
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
			MeshData.IndexData.push_back(CenterPoint);
			MeshData.IndexData.push_back(Index + i);
			MeshData.IndexData.push_back(Index + i + 1);
		}
	}
}
