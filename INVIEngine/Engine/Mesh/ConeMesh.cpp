#include "ConeMesh.h"

void CConeMesh::BeginInit()
{
	CMesh::BeginInit();
}

void CConeMesh::Tick(float DeltaTime)
{
	CMesh::Tick(DeltaTime);
}

CConeMesh::~CConeMesh()
{
}

void CConeMesh::Init()
{
	CMesh::Init();
}

void CConeMesh::PreDraw(float DeltaTime)
{
	CMesh::PreDraw(DeltaTime);
}

void CConeMesh::Draw(float DeltaTime)
{
	CMesh::Draw(DeltaTime);
}

void CConeMesh::PostDraw(float DeltaTime)
{
	CMesh::PostDraw(DeltaTime);
}

void CConeMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}

CConeMesh* CConeMesh::CreateMesh(float InBottomRadius, float InHeight, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	FMeshRendingData MeshData;

	// 半径间隔
	float RadiusInterval =  -InBottomRadius / static_cast<float>(InHeightSubdivision);

	// 高度间隔
	float HeightInterval = InHeight / static_cast<float>(InHeightSubdivision);

	// 弧度
	float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 0.5f * InHeight, 0.f), XMFLOAT4(Colors::Pink)));

	uint32_t Index = MeshData.VertexData.size();

	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i;
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),//x
					Y,//y
					Radius * sinf(j * BetaValue)), //z
				XMFLOAT4(j * 0.02f, i * 0.1f, (i + j) * 0.4f, 1.0f)));
		}
	}

	//添加中点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, -0.5f * InHeight, 0.f), XMFLOAT4(Colors::Azure)));

	//绘制index模型
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	float BaseIndex = 1;
	float VertexCircleNum = InAxialSubdivision + 1;
	//绘制腰围
	for (uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//我们绘制的是四边形
			//三角形1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//三角形2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	//绘制南极
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;
	BaseIndex = SouthBaseIndex - VertexCircleNum;
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)
	{
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + Index);
		MeshData.IndexData.push_back(BaseIndex + Index + 1);
	}


	CConeMesh* ConeMesh = new CConeMesh();
	ConeMesh->BuildMesh(&MeshData);

	ConeMesh->Init();

	return ConeMesh;
}
