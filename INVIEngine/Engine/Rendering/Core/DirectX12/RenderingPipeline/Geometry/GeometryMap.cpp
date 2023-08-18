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
	// �жϵ�ǰģ���Ƿ��Ѿ�����ӹ���
	if (!bRenderingDataExistence(Mesh))
	{
		DescribeMeshRenderingData.push_back(FRenderingData());
		FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];

		InRenderingData.Mesh = Mesh;
		// ��¼��������
		InRenderingData.IndexSize = MeshData.IndexData.size();
		InRenderingData.VertexSize = MeshData.VertexData.size();
		// ��¼����ƫ��
		InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
		InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

		// һ�ָ�Ч�����ݲ��뷽ʽ
		// �����ϲ�
		MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
		// ����ϲ�
		MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshData.VertexData.begin(), MeshData.VertexData.end());
	}
}

void FGeometry::Build()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����ģ��

	UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

	// ����������
	ANALYSIS_RESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));	// ����һ�������ƵĻ�����
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), VertexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ConstructBuffer::FConstructDefaultBuffer ConstructDefaultBuffer;

	GPUVertexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempVertexBufferPtr, MeshRenderingData.VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer.ConstructDefaultBuffer(TempIndexBufferPtr, MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ANALYSIS_RESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));	// ����һ�������ƵĻ�����
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
	// +1 ��ʾ������ĳ���������
	DescriptorHeap.Build(GetDrawObjectCount() + 1);
}

UINT FGeometryMap::GetDrawObjectCount()
{
	// Ŀǰ��д���ɵ�һ��
	return Geometries[0].GetDrawObjectCount();
}

void FGeometryMap::BuildConstantBuffer()
{
	// ��������������
	ObjectConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), 1);

	// �����Ѿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// ��������������
	ObjectConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawObjectCount());
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	// ��������������
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	// �����Ѿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	// ��������������
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawObjectCount(), GetDrawObjectCount());
}

void FGeometryMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	XMMATRIX ProjectionMatrix = XMLoadFloat4x4(&viewport_info.ProjectionMatrix);	// ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&viewport_info.ViewMatrix);				// �ӿھ���
	for (auto& geometry : Geometries)
	{
		for (int i = 0; i < geometry.second.DescribeMeshRenderingData.size(); i++)
		{
			// ����ģ��λ��

			FRenderingData& renderingData = geometry.second.DescribeMeshRenderingData[i];

			XMMATRIX MatrixWorld = XMLoadFloat4x4(&renderingData.WorldMatrix);

			FObjectTransformation OBJTransformation;
			XMStoreFloat4x4(&OBJTransformation.World, XMMatrixTranspose(MatrixWorld));
			ObjectConstantBufferViews.GetConstant()->Update(i, &OBJTransformation);

		}
	}

	XMMATRIX ViewProjection = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// �洢֮ǰ�ǵöԾ������ת��

	ViewportConstantBufferViews.GetConstant()->Update(0, &ViewportTransformation);
	
}
