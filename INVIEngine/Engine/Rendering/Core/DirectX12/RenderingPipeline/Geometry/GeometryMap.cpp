#include "GeometryMap.h"

#include "Core/Viewport/ViewportInfo.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/Core/ObjectTransformation.h"
#include "Rendering/Core/RenderingResourcesUpdate.h"
#include "Rendering/Core/Buffer/ConstructBuffer.h"

bool FGeometry::bRenderingDataExistence(CMesh* InKey)
{
	for(auto& temp : DescribeMeshRenderingData)
	{
		return temp.Mesh == InKey;
	}

	return false;
}

void FGeometry::BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData)
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

	UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

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

/**
 * \brief //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
FGeometryMap::FGeometryMap()
{
	Geometries.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData)
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
	ObjectConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), 1);

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
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawObjectCount(), GetDrawObjectCount());
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

			XMMATRIX MatrixWorld = XMLoadFloat4x4(&renderingData.WorldMatrix);

			FObjectTransformation OBJTransformation;
			XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(MatrixWorld));
			ObjectConstantBufferViews.GetConstant()->Update(i, &OBJTransformation);

		}
	}

	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// 存储之前记得对矩阵进行转置

	ViewportConstantBufferViews.GetConstant()->Update(0, &ViewportTransformation);
	
}
