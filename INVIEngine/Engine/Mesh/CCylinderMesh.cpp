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

void CCylinderMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}

CCylinderMesh* CCylinderMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight,
	uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	FMeshRendingData MeshData;

	// 半径间隔
	float RadiusInterval = (InTopRadius - InBottomRadius) / static_cast<float>(InHeightSubdivision);

	// 高度间隔
	float HeightInterval = InHeight / static_cast<float>(InHeightSubdivision);

	// 弧度
	float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	for (uint32_t i = 0; i <= InHeightSubdivision; ++i)
	{
		float Y = (0.5f * InHeight) - HeightInterval * i;
		float Radius = InTopRadius + i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(BetaValue),	// x
					Y,							// Y
					Radius * sinf(BetaValue)	// z
				),
				XMFLOAT4(
					j * 0.02f, i * 0.1f, (i + j) * 0.4f, 1.0f
				)
			));
		}
	}

	CCylinderMesh* cylinderMesh = new CCylinderMesh();
	cylinderMesh->BuildMesh(&MeshData);

	cylinderMesh->Init();

	return cylinderMesh;
}
