#include "GeometryMap.h"

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
