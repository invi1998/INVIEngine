#include "SphereMesh.h"


CSphereMesh::~CSphereMesh()
{
}

void CSphereMesh::BeginInit()
{
	CMesh::BeginInit();
}

void CSphereMesh::Tick(float DeltaTime)
{
	CMesh::Tick(DeltaTime);
}

void CSphereMesh::Init()
{
	CMesh::Init();
}

void CSphereMesh::PreDraw(float DeltaTime)
{
	CMesh::PreDraw(DeltaTime);
}

void CSphereMesh::Draw(float DeltaTime)
{
	CMesh::Draw(DeltaTime);
}

void CSphereMesh::PostDraw(float DeltaTime)
{
	CMesh::PostDraw(DeltaTime);
}

void CSphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);

	Init();
}

void CSphereMesh::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{

	float Theta = XM_2PI / static_cast<float>(InHeightSubdivision);
	float Beta = XM_PI / static_cast<float>(InAxialSubdivision);

	// ����
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(0.98f, 0.12f, 0.23f, 1.0f)));

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
				XMFLOAT4(
					i * 0.01f, j * 0.1f, (i + j) * 0.2f, 1.0f
				)
			));

			int TopIndex = MeshData.VertexData.size() - 1;
			// �������ϸõ�ķ���
			XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexData[TopIndex].Position);
			XMStoreFloat3(&MeshData.VertexData[TopIndex].Normal, XMVector3Normalize(Pos));
		}
	}

	// �ײ�
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(0.18f, 0.12f, 0.93f, 1.0f)));


	// ������������ (���Ʊ�����

	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
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
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + i);
		MeshData.IndexData.push_back(BaseIndex + i + 1);
	}
}
