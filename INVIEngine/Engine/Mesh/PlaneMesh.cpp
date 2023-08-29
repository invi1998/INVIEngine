#include "PlaneMesh.h"

void GPlaneMesh::BeginInit()
{
	GMesh::BeginInit();
}

void GPlaneMesh::Tick(float DeltaTime)
{
	GMesh::Tick(DeltaTime);
}

GPlaneMesh::~GPlaneMesh()
{
}

void GPlaneMesh::Init()
{
	GMesh::Init();
}

void GPlaneMesh::PreDraw(float DeltaTime)
{
	GMesh::PreDraw(DeltaTime);
}

void GPlaneMesh::Draw(float DeltaTime)
{
	GMesh::Draw(DeltaTime);
}

void GPlaneMesh::PostDraw(float DeltaTime)
{
	GMesh::PostDraw(DeltaTime);
}

void GPlaneMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	GMesh::BuildMesh(InRenderingData);
}

void GPlaneMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{

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
				XMFLOAT4(Colors::Gray), XMFLOAT3(0.f, 0.f, 1.f)));
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
	
}
