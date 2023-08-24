#include "GeometryMap.h"

#include "Core/Viewport/ViewportInfo.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/Core/Mesh.h"
#include "Mesh/Core/ObjectTransformation.h"
#include "Rendering/Core/RenderingResourcesUpdate.h"
#include "Rendering/Core/Buffer/ConstructBuffer.h"

bool FGeometry::bRenderingDataExistence(GMesh* InKey)
{
	for(const auto& temp : DescribeMeshRenderingData)
	{
		return temp.Mesh == InKey;
	}

	return false;
}

void FGeometry::BuildMesh(GMesh* Mesh, const FMeshRenderingData& MeshData)
{
	// 判断当前模型是否已经被添加过了
	if (!bRenderingDataExistence(Mesh))
	{
		DescribeMeshRenderingData.push_back(FRenderingData());
		FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];

		InRenderingData.Mesh = Mesh;
		// 记录顶点数据
		InRenderingData.IndexSize = MeshData.IndexData.size();
		InRenderingData.VertexSize = MeshData.VertexData.size();
		// 记录数据偏移
		InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
		InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

		// 一种高效的数据插入方式
		// 索引合并
		MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
		// 顶点合并
		MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshData.VertexData.begin(), MeshData.VertexData.end());
	}
}

void FGeometry::Build()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 构建模型

	const UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	const UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

	// 创建缓冲区
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// 创建一个二进制的缓冲区
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), VertexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ConstructBuffer::FConstructDefaultBuffer ConstructDefaultBuffer;

	GPUVertexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempVertexBufferPtr, MeshRenderingData.VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempIndexBufferPtr, MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// 创建一个二进制的缓冲区
}

UINT FGeometry::GetDrawObjectCount() const
{
	return DescribeMeshRenderingData.size();
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv;

	vbv.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	vbv.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	vbv.StrideInBytes = sizeof(FVertex);

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW ibv;

	ibv.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	ibv.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
	ibv.Format = DXGI_FORMAT_R16_UINT;

	return ibv;
}

/**
 * \brief //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
FGeometryMap::FGeometryMap()
{
	Geometries.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::BuildMesh(GMesh* Mesh, const FMeshRenderingData& MeshData)
{
	FGeometry &Geometry = Geometries[0];

	Geometry.BuildMesh(Mesh, MeshData);
}

void FGeometryMap::Build()
{
	for (auto & [index, Geometry] : Geometries)
	{
		Geometry.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	// +1 表示摄像机的常量缓冲区
	DescriptorHeap.Build(GetDrawObjectCount() + 1);
}

UINT FGeometryMap::GetDrawObjectCount()
{
	// 目前先写死成第一个
	return Geometries[0].GetDrawObjectCount();
}

void FGeometryMap::BuildConstantBuffer()
{
	// 创建常量缓冲区
	ObjectConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectCount());

	// 描述堆句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// 构建常量缓冲区
	ObjectConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawObjectCount());
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	// 创建常量缓冲区
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	// 描述堆句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// 构建常量缓冲区
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, 1, GetDrawObjectCount());
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// 投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// 视口矩阵
	for (auto& geometry : Geometries)
	{
		for (int i = 0; i < geometry.second.DescribeMeshRenderingData.size(); i++)
		{
			// 更新模型位置
			FRenderingData& renderingData = geometry.second.DescribeMeshRenderingData[i];
			XMFLOAT3& Position = renderingData.Mesh->GetPosition();

			// 拿到3个方向向量
			XMFLOAT3 RightVector = renderingData.Mesh->GetRightVector();
			XMFLOAT3 UpVector = renderingData.Mesh->GetUpVector();
			XMFLOAT3 ForwardVector = renderingData.Mesh->GetForwardVector();

			// 构造模型world
			renderingData.WorldMatrix = {
				RightVector.x, UpVector.x, ForwardVector.x, 0.f,
				RightVector.y, UpVector.y, ForwardVector.y, 0.f,
				RightVector.z, UpVector.z, ForwardVector.z, 0.f,
				Position.x, Position.y, Position.z, 1.f
			};

			XMMATRIX MatrixWorld = XMLoadFloat4x4(&renderingData.WorldMatrix);

			FObjectTransformation OBJTransformation;
			XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(MatrixWorld));
			ObjectConstantBufferViews.Update(i, &OBJTransformation);

		}
	}

	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// 存储之前记得对矩阵进行转置

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);
	
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	// 渲染视口
	DrawViewport(DeltaTime);

	// 渲染模型
	DrawMesh(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawObjectCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// 更新b1寄存器 (视口是后渲染，所以放在寄存器1中）
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& geometry : Geometries)
	{
		// 获取顶点/索引缓冲区视图
		D3D12_VERTEX_BUFFER_VIEW vbv = geometry.second.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = geometry.second.GetIndexBufferView();

		for (size_t i = 0; i < geometry.second.DescribeMeshRenderingData.size(); i++)
		{
			CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

			FRenderingData& renderingData = geometry.second.DescribeMeshRenderingData[i];

			// 设置索引视图
			GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

			// 绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
			GetD3dGraphicsCommandList()->IASetVertexBuffers(
				0,		// 指定起始的顶点缓冲区槽号 `StartSlot`，这个值通常为 0 值范围 0-15
				1,		// 指定要绑定的顶点缓冲区数量 NumViews，这个值必须小于或等于硬件支持的最大顶点缓冲区数量
				&vbv	// 使用顶点缓冲区视图数组 pViews 来描述每个顶点缓冲区的位置、大小和布局等信息。
			);

			// 指定图元类型（点，线，面） 下面设置为 绘制由三角形构成的列表
			GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			DesHandle.Offset(i, DescriptorOffset);
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesHandle);

			// 上述步骤只是在提交数据到GPU，并不是在渲染模型

			// 绘制图元（真实渲染模型）
			GetD3dGraphicsCommandList()->DrawIndexedInstanced(
				renderingData.IndexSize,						// 顶点数量
				1,												// 表示要绘制的实例数。如果只需要绘制单个实例，则可以将此参数设置为 1。
				renderingData.IndexOffsetPosition,				// 表示从索引缓冲区中哪个位置开始读取索引数据。通常情况下，这个值为 0。
				renderingData.VertexOffsetPosition,				// 表示每个顶点在顶点缓冲区中相对于起始位置的偏移量。如果顶点数据是连续排列的，则可以将此参数设置为 0。
				0												// 表示从第几个实例开始绘制。如果只需要绘制单个实例，则可以将此参数设置为 0。
			);

		}
	}
}
