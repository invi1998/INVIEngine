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

	// 顶部
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(0.98f, 0.12f, 0.23f, 1.0f)));

	for (size_t i = 1; i < InAxialSubdivision; ++i)
	{
		float BetaValue = i * Beta;

		for (size_t j = 0; j <= InHeightSubdivision; ++j)
		{
			float ThetaValue = j * Theta;

			// 球面上该顶点坐标
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
			// 求球面上该点的法线
			XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexData[TopIndex].Position);
			XMStoreFloat3(&MeshData.VertexData[TopIndex].Normal, XMVector3Normalize(Pos));
		}
	}

	// 底部
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(0.18f, 0.12f, 0.93f, 1.0f)));


	// 构建顶点索引 (绘制北极）

	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		// 因为DX是左手螺旋定则，所以需要逆时针绘制顶点，法线才能朝外
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	uint32_t BaseIndex = 1;
	// 绘制中间区域的点
	uint32_t VertexCircleNum = InAxialSubdivision + 1;
	for (uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			// 在球体的南北极中间绘制的面是四边形(而一个四边形又是由两个三角形组成的
			// 三角形1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			// 三角形2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	// 绘制南极
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;		// 极点索引
	BaseIndex = SouthBaseIndex - VertexCircleNum;			// 极平面绘制点的起始索引（极点减去经线数量）
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		// 因为DX是左手螺旋定则，所以需要逆时针绘制顶点，法线才能朝外
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + i);
		MeshData.IndexData.push_back(BaseIndex + i + 1);
	}
}
