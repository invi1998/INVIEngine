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
	// ���㶥�������ֽڴ�С
	UINT VertexSize = InRenderingData->VertexData.size() * sizeof(FVertex);
	UINT IndexSize = InRenderingData->IndexData.size() * sizeof(uint16_t);

	// ����������
	ANALYSIS_RESULT(D3DCreateBlob(VertexSize, &CPUVertexBufferPtr));	// ����һ�������ƵĻ�����
	memccpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), 0, VertexSize);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSize, &CPUIndexBufferPtr));	// ����һ�������ƵĻ�����
}

FMesh* FMesh::CreateMesh(const FMeshRendingData* InRenderingData)
{
	FMesh* InMesh = new FMesh();

	InMesh->BuildMesh(InRenderingData);

	return InMesh;
}
