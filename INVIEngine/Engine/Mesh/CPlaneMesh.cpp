#include "CPlaneMesh.h"

void CPlaneMesh::BeginInit()
{
	CMesh::BeginInit();
}

void CPlaneMesh::Tick(float DeltaTime)
{
	CMesh::Tick(DeltaTime);
}

CPlaneMesh::~CPlaneMesh()
{
}

void CPlaneMesh::Init()
{
	CMesh::Init();
}

void CPlaneMesh::PreDraw(float DeltaTime)
{
	CMesh::PreDraw(DeltaTime);
}

void CPlaneMesh::Draw(float DeltaTime)
{
	CMesh::Draw(DeltaTime);
}

void CPlaneMesh::PostDraw(float DeltaTime)
{
	CMesh::PostDraw(DeltaTime);
}

void CPlaneMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}

CPlaneMesh* CPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	FMeshRendingData MeshData;

	auto SubdivideValue = [&](float InValue, uint32_t InSubdivideValue)-> float
	{
		if (InSubdivideValue <= 1) return InValue;

		return InHeight / (static_cast<float>(InSubdivideValue) - 1);
	};

	float CenterHeight = 0.5f * InHeight;
	float CenterWidth = 0.5f * InWidth;

	// 获取细分值
	float HeightSubdivideValue = SubdivideValue(InHeight, InHeightSubdivide);
	float WidthSubdivideValue = SubdivideValue(InWidth, InWidthSubdivide);

	// 顶点位置
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CenterHeight - i * HeightSubdivideValue;
		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CenterWidth - j * WidthSubdivideValue;
			MeshData.VertexData.push_back(FVertex(XMFLOAT3(
				X,
				0,
				Z
			),
				XMFLOAT4(Colors::Orchid)));
		}
	}

	// 顶点索引
	for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
	{
		float Z = CenterHeight - i * HeightSubdivideValue;
		for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
		{
			// MeshData.IndexData.push_back(i * InWidthSubdivide + j);
			// MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			// MeshData.IndexData.push_back((i+1) * InWidthSubdivide + j);
			// 
			// MeshData.IndexData.push_back((i+1) * InWidthSubdivide + j);
			// MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			// MeshData.IndexData.push_back((i+1) * InWidthSubdivide + j + 1);

			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j);

			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
		}
	}

	CPlaneMesh* PlaneMesh = new CPlaneMesh();
	PlaneMesh->BuildMesh(&MeshData);

	PlaneMesh->Init();

	return PlaneMesh;
}
