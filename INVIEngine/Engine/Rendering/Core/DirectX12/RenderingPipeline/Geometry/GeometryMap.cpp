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

	const UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	const UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

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
	ObjectConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectCount());

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
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, 1, GetDrawObjectCount());
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
			XMFLOAT3& Position = renderingData.Mesh->GetPosition();

			// �õ�3����������
			XMFLOAT3 RightVector = renderingData.Mesh->GetRightVector();
			XMFLOAT3 UpVector = renderingData.Mesh->GetUpVector();
			XMFLOAT3 ForwardVector = renderingData.Mesh->GetForwardVector();

			// ����ģ��world
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
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProjection));	// �洢֮ǰ�ǵöԾ������ת��

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);
	
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	// ��Ⱦ�ӿ�
	DrawViewport(DeltaTime);

	// ��Ⱦģ��
	DrawMesh(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(static_cast<INT>(GetDrawObjectCount()), DescriptorOffset);
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);		// ����b1�Ĵ��� (�ӿ��Ǻ���Ⱦ�����Է��ڼĴ���1�У�
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& geometry : Geometries)
	{
		// ��ȡ����/������������ͼ
		D3D12_VERTEX_BUFFER_VIEW vbv = geometry.second.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = geometry.second.GetIndexBufferView();

		for (size_t i = 0; i < geometry.second.DescribeMeshRenderingData.size(); i++)
		{
			CD3DX12_GPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

			FRenderingData& renderingData = geometry.second.DescribeMeshRenderingData[i];

			// ����������ͼ
			GetD3dGraphicsCommandList()->IASetIndexBuffer(&ibv);

			// ����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
			GetD3dGraphicsCommandList()->IASetVertexBuffers(
				0,		// ָ����ʼ�Ķ��㻺�����ۺ� `StartSlot`�����ֵͨ��Ϊ 0 ֵ��Χ 0-15
				1,		// ָ��Ҫ�󶨵Ķ��㻺�������� NumViews�����ֵ����С�ڻ����Ӳ��֧�ֵ���󶥵㻺��������
				&vbv	// ʹ�ö��㻺������ͼ���� pViews ������ÿ�����㻺������λ�á���С�Ͳ��ֵ���Ϣ��
			);

			// ָ��ͼԪ���ͣ��㣬�ߣ��棩 ��������Ϊ �����������ι��ɵ��б�
			GetD3dGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			DesHandle.Offset(i, DescriptorOffset);
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesHandle);

			// ��������ֻ�����ύ���ݵ�GPU������������Ⱦģ��

			// ����ͼԪ����ʵ��Ⱦģ�ͣ�
			GetD3dGraphicsCommandList()->DrawIndexedInstanced(
				renderingData.IndexSize,						// ��������
				1,												// ��ʾҪ���Ƶ�ʵ���������ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 1��
				renderingData.IndexOffsetPosition,				// ��ʾ���������������ĸ�λ�ÿ�ʼ��ȡ�������ݡ�ͨ������£����ֵΪ 0��
				renderingData.VertexOffsetPosition,				// ��ʾÿ�������ڶ��㻺�������������ʼλ�õ�ƫ��������������������������еģ�����Խ��˲�������Ϊ 0��
				0												// ��ʾ�ӵڼ���ʵ����ʼ���ơ����ֻ��Ҫ���Ƶ���ʵ��������Խ��˲�������Ϊ 0��
			);

		}
	}
}
