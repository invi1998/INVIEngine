#pragma once

#include "EngineMinimal.h"
#include "RenderingData.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/MeshType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/ConstantBuffer/ConstantBufferViews.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

struct FGeometry : IDirectXDeviceInterface_Struct
{
public:
	bool bRenderingDataExistence(CMesh* InKey);
	void BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData);

	// ����ģ��
	void Build();

	UINT GetDrawObjectCount() const;

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU ���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU ����������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU ���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU ����������

	// ��ʱ���������������ǽ�CPU��buffer���ݸ��Ƶ�GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// ��ʱ ���㻺����
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// ��ʱ ����������

	FMeshRenderingData MeshRenderingData;

	std::vector<FRenderingData> DescribeMeshRenderingData;					// ��Ⱦ����
};

// ����ģ��ӳ��׶�
struct FGeometryMap : IDirectXDeviceInterface_Struct
{
public:
	FGeometryMap();
	void BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData);

	void Build();

	void BuildDescriptorHeap();

	UINT GetDrawObjectCount();

	void BuildConstantBuffer();

protected:
	map<int, FGeometry> Geometries;				// ������
	FDirectXDescriptorHeap DescriptorHeap;		// ������
	FConstantBufferViews ObjectConstantBufferViews;	// ������������
};

