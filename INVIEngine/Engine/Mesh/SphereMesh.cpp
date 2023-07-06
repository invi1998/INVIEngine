#include "SphereMesh.h"

CSphereMesh::CSphereMesh()
	: Radius(1.0f), AxialSubdivision(10), HeightSubdivision(10)
{
	FMeshRendingData MeshData;



	Theta = XM_2PI / static_cast<float>(HeightSubdivision);
	Beta = XM_2PI / static_cast<float>(AxialSubdivision);

	for (size_t i = 0; i < AxialSubdivision; i++)
	{
		float BetaValue = (i + 1) * Beta;

		for (size_t j = 0; j < HeightSubdivision; j++)
		{
			float ThetaValue = (j + 1) * Theta;

			// �����ϸö�������
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * sinf(BetaValue) * cosf(ThetaValue),
					Radius * cosf(BetaValue),
					Radius * sinf(BetaValue) * sinf(ThetaValue)
				),
				XMFLOAT4(
					i * 0.01f, j * 0.1f, (j + j)*0.2f, 1.0f 
				)
			));

			int TopIndex = MeshData.VertexData.size() - 1;
			// �������ϸõ�ķ���
			XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexData[TopIndex].Position);
			XMStoreFloat3(&MeshData.VertexData[TopIndex].Normal, XMVector3Normalize(Pos));
		}
	}

	// ������������ (���Ʊ�����

	for (uint32_t i = 0; i < AxialSubdivision; ++i)
	{
		// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	uint32_t BaseIndex = 1;
	// �����м�����ĵ�
	uint32_t VertexCircleNum = AxialSubdivision + 1;
	for (uint32_t i = 0; i < HeightSubdivision - 2; ++i)
	{
		for (uint32_t j = 0; j < AxialSubdivision; ++j)
		{
			// ��������ϱ����м���Ƶ������ı���(��һ���ı���������������������ɵ�
			// ������1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i+1) * VertexCircleNum + j);
			// ������2
			MeshData.IndexData.push_back(BaseIndex + (i+1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i+1) * VertexCircleNum + j + 1);
		}
	}

	// �����ϼ�
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;		// ��������
	BaseIndex = SouthBaseIndex - VertexCircleNum;			// ��ƽ����Ƶ����ʼ�����������ȥ����������
	for (uint32_t i = 0; i < AxialSubdivision; ++i)
	{
		// ��ΪDX��������������������Ҫ��ʱ����ƶ��㣬���߲��ܳ���
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + i);
		MeshData.IndexData.push_back(BaseIndex + i + 1);
	}

}

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

void CSphereMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}
