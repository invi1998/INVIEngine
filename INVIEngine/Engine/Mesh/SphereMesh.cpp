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

			// 球面上该顶点坐标
			FVertex Vertex(
				XMFLOAT3(
					Radius * sinf(BetaValue) * cosf(ThetaValue),
					Radius * cosf(BetaValue),
					Radius * sinf(BetaValue) * sinf(ThetaValue)
				),
				XMFLOAT4(
					i * 0.01f, j * 0.1f, (j + j)*0.2f, 1.0f 
				)
			);

			// 求球面上该点的法线
			XMVECTOR Pos = XMLoadFloat3(&Vertex.Position);
		}

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
