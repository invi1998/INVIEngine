#include "BoxMesh.h"

CBoxMesh::CBoxMesh()
{
}

CBoxMesh::~CBoxMesh()
{
}

void CBoxMesh::Init()
{
	Super::Init();
}

void CBoxMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}

void CBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

CBoxMesh* CBoxMesh::CreateMesh(float InHeight, float InWidth, float InDepth)
{
	FMeshRendingData MeshData;

	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;

	//�������ǵĶ���
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth), XMFLOAT4(Colors::Pink)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, -CDepth), XMFLOAT4(Colors::Bisque)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, -CDepth), XMFLOAT4(Colors::Khaki)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, -CDepth), XMFLOAT4(Colors::Silver)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, CDepth), XMFLOAT4(Colors::Yellow)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, CDepth), XMFLOAT4(Colors::RoyalBlue)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, CDepth), XMFLOAT4(Colors::DarkRed)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, CDepth), XMFLOAT4(Colors::Teal)));

	//�������ǵ�����
	//ǰ
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

	//��
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

	//��
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

	//��
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

	//��
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

	//��
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);

	CBoxMesh* Box = new CBoxMesh();

	Box->BuildMesh(&MeshData);

	Box->Init();

	return Box;

}
