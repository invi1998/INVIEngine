#include "CCylinderMesh.h"

void CCylinderMesh::BeginInit()
{
	CMesh::BeginInit();
}

void CCylinderMesh::Tick(float DeltaTime)
{
	CMesh::Tick(DeltaTime);
}

CCylinderMesh::~CCylinderMesh()
{
}

void CCylinderMesh::Init()
{
	CMesh::Init();
}

void CCylinderMesh::PreDraw(float DeltaTime)
{
	CMesh::PreDraw(DeltaTime);
}

void CCylinderMesh::Draw(float DeltaTime)
{
	CMesh::Draw(DeltaTime);
}

void CCylinderMesh::PostDraw(float DeltaTime)
{
	CMesh::PostDraw(DeltaTime);
}

void CCylinderMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}

CCylinderMesh* CCylinderMesh::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius, float InHeight,
	uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{

	// 半径间隔
	float RadiusInterval = (InTopRadius - InBottomRadius) / static_cast<float>(InHeightSubdivision);

	// 高度间隔
	float HeightInterval = InHeight / static_cast<float>(InHeightSubdivision);

	// 弧度
	float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	// 腰部顶点
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
				XMFLOAT4(
					j * 0.02f, i * 0.1f, (i + j) * 0.4f, 1.0f
				)
			));
		}
	}

	// 绘制圆柱腰围 index
	uint32_t VertexCircleNum = InAxialSubdivision + 1;
	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			// 南北极中间绘制的面是四边形(而一个四边形又是由两个三角形组成的

			// 法线远离摄像机
			// 三角形1
			// MeshData.IndexData.push_back(i * VertexCircleNum + j);
			// MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j);
			// MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			// // 三角形2
			// MeshData.IndexData.push_back(i * VertexCircleNum + j);
			// MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			// MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);

			 // 法线朝向自己
			 // 三角形1
			 MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			 MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j);
			 MeshData.IndexData.push_back(i * VertexCircleNum + j);
			 // 三角形2
			 MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);
			 MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			 MeshData.IndexData.push_back(i * VertexCircleNum + j);
		}
	}

	// 构建顶部
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
				XMFLOAT4(
					0.12f, 0.21f, 0.84f, 1.0f
				)
			));
		}

		MeshData.VertexData.push_back(FVertex(
			XMFLOAT3(
				0.f,	// x
				Y,	// Y
				0.f	// z
			),
			XMFLOAT4(
				0.92f, 0.1f, 0.21f, 1.0f
			)
		));

		// index
		float CenterPoint = MeshData.VertexData.size() - 1;
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			// 因为DX是左手螺旋定则，所以需要逆时针绘制顶点，法线才能朝外
			MeshData.IndexData.push_back(CenterPoint);
			MeshData.IndexData.push_back(Index + i + 1);
			MeshData.IndexData.push_back(Index + i);
		}

	}

	// 构建底部
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
				XMFLOAT4(
					0.12f, 0.81f, 0.4f, 1.0f
				)
			));
		}

		MeshData.VertexData.push_back(FVertex(
			XMFLOAT3(
				0.f,	// x
				Y,	// Y
				0.f	// z
			),
			XMFLOAT4(
				0.92f, 0.1f, 0.21f, 1.0f
			)
		));

		// index
		float CenterPoint = MeshData.VertexData.size() - 1;
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			// 因为DX是左手螺旋定则，所以需要逆时针绘制顶点，法线才能朝外
			MeshData.IndexData.push_back(CenterPoint);
			MeshData.IndexData.push_back(Index + i);
			MeshData.IndexData.push_back(Index + i + 1);
		}
	}
}
