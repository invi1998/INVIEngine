#include "Mesh.h"
#include "EngineMinimal.h"

FMesh::FMesh()
{
}

FMesh::~FMesh()
{
}

void FMesh::Init()
{
	IRenderingInterface::Init();
}

void FMesh::Draw(float DeltaTime)
{
	IRenderingInterface::Draw(DeltaTime);
}

void FMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	// 计算顶点数据字节大小
	UINT VertexSize = InRenderingData->VertexData.size() * sizeof(FVertex);
	UINT IndexSize = InRenderingData->IndexData.size() * sizeof(uint16_t);

	// 创建缓冲区
	ANALYSIS_RESULT(D3DCreateBlob(VertexSize, &CPUVertexBufferPtr));	// 创建一个二进制的缓冲区
	memccpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), 0, VertexSize);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSize, &CPUIndexBufferPtr));	// 创建一个二进制的缓冲区
}

FMesh* FMesh::CreateMesh(const FMeshRendingData* InRenderingData)
{
	FMesh* InMesh = new FMesh();

	InMesh->BuildMesh(InRenderingData);

	return InMesh;
}
