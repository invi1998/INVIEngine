#include "BoxMesh.h"

FBoxMesh::FBoxMesh()
{
}

FBoxMesh::~FBoxMesh()
{
}

void FBoxMesh::Init()
{
	Super::Init();
}

void FBoxMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);
}

void FBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

FBoxMesh* FBoxMesh::CreateMesh()
{
	// 构建顶点，索引
	FMeshRendingData MeshRenderingData(
		std::vector<uint16_t>{
			// 前
			0, 1, 2,
			0, 2, 3,
			// 后
			4, 6, 5,
			4, 7, 6,
			// 左
			4, 5, 1,
			4, 1, 0,
			// 右
			3, 2, 6,
			3, 6, 7,
			// 上
			1, 5, 6,
			1, 6, 2,
			// 下
			4, 0, 3,
			4, 3, 7
		},
		{
			FVertex(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(0.76f, 0.1f, 0.12f, 1.f)),
			FVertex(XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT4(0.16f, 0.61f, 0.12f, 1.f)),
			FVertex(XMFLOAT3(1.f, 1.f, 0.f), XMFLOAT4(0.26f, 0.51f, 0.212f, 1.f)),
			FVertex(XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT4(0.36f, 0.41f, 0.42f, 1.f)),
			FVertex(XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT4(0.46f, 0.31f, 0.12f, 1.f)),
			FVertex(XMFLOAT3(0.f, 1.f, 1.f), XMFLOAT4(0.56f, 0.21f, 0.22f, 1.f)),
			FVertex(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.76f, 0.11f, 0.12f, 1.f)),
			FVertex(XMFLOAT3(1.f, 0.f, 1.f), XMFLOAT4(0.56f, 0.11f, 0.12f, 1.f)),
		});

	FBoxMesh* Box = new FBoxMesh();

	Box->BuildMesh(&MeshRenderingData);

	Box->Init();

	return Box;

}
