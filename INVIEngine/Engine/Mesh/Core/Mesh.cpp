#include "Mesh.h"
#include "EngineMinimal.h"

FMesh::FMesh()
	: IndexSize(0), VertexSizeInBytes(0), VertexStrideInBytes(0), IndexSizeInBytes(0), IndexFormat(DXGI_FORMAT_R16_UINT)
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

	// 将我们的顶点缓冲区和渲染流水线绑定

	// 获取顶点缓冲区视图
	D3D12_VERTEX_BUFFER_VIEW vbv = GetVertexBufferView();
	// 绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
	GetD3dGraphicsCommandList()->IASetVertexBuffers(
		0,		// 指定起始的顶点缓冲区槽号 `StartSlot`，这个值通常为 0 值范围 0-15
		1,		// 指定要绑定的顶点缓冲区数量 NumViews，这个值必须小于或等于硬件支持的最大顶点缓冲区数量
		&vbv	// 使用顶点缓冲区视图数组 pViews 来描述每个顶点缓冲区的位置、大小和布局等信息。
	);

	D3D12_INDEX_BUFFER_VIEW ibv = GetIndexBufferView();
	GetD3dGraphicsCommandList()->IASetIndexBuffer(
		&ibv	// 索引缓冲区视图
	);

	// 上述步骤只是在提交数据到GPU，并不是在渲染模型

	// 指定图元类型（点，线，面） 下面设置为 绘制由三角形构成的列表
	GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 绘制图元（真实渲染模型）
	GetD3dGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,		// 顶点数量
		1,				// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
		0,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
		0,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
		0				// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
	);
}

void FMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	IndexSize = InRenderingData->IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);

	// 计算顶点数据字节大小
	VertexSizeInBytes = InRenderingData->VertexData.size() * sizeof(FVertex);
	IndexSizeInBytes = InRenderingData->IndexData.size() * sizeof(uint16_t);

	// 创建缓冲区
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// 创建一个二进制的缓冲区
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

	GPUVertexBufferPtr = ConstructDefaultBuffer(TempVertexBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(TempIndexBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// 创建一个二进制的缓冲区
}

FMesh* FMesh::CreateMesh(const FMeshRendingData* InRenderingData)
{
	FMesh* InMesh = new FMesh();

	InMesh->BuildMesh(InRenderingData);

	return InMesh;
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	vbv.SizeInBytes = VertexSizeInBytes;
	vbv.StrideInBytes = VertexStrideInBytes;

	return vbv;

}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW ibv;

	ibv.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	ibv.SizeInBytes = IndexSizeInBytes;
	ibv.Format = IndexFormat;

	return ibv;
}
