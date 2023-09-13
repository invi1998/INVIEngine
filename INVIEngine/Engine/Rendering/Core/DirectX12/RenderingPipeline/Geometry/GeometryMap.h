#pragma once

#include "EngineMinimal.h"
#include "RenderingData.h"
#include "Component/Light/Core/LightConstantBuffer.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Material/Core/MaterialConstantBuffer.h"
#include "Mesh/Core/MeshType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/ConstantBuffer/ConstantBufferViews.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

class CMeshComponent;
struct FViewportInfo;

struct FGeometry : IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;
public:
	bool bRenderingDataExistence(CMeshComponent* InKey);
	void BuildMesh(CMeshComponent* Mesh, const FMeshRenderingData& MeshData);

	// ����ģ��
	void Build();

	UINT GetDrawObjectCount() const;

	// ��ȡ����buff��ͼ
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	// ��ȡ����index��������ͼ
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

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
	void BuildMesh(CMeshComponent* Mesh, const FMeshRenderingData& MeshData);

	void Build();

	void BuildDescriptorHeap();

	// ����ģ�͵Ļ�������
	UINT GetDrawMeshCount();

	// ģ�Ͳ��ʵĻ�������
	UINT GetDrawMaterialCount();

	// �ƹ�Ļ�������
	UINT GetDrawLightCount();

	// ��������ģ�͵�buff������
	void BuildMeshConstantBuffer();

	// ����ģ�Ͳ��ʵ�buff������
	void BuildMaterialConstantBuffer();

	// �����ƹ��buff������
	void BuildLightConstantBuffer();

	// �����ӿ�buffer
	void BuildViewportConstantBuffer();

	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	ID3D12DescriptorHeap* GetHeap() const { return DescriptorHeap.GetHeap(); }

private:
	// ��Ⱦ�ӿ�
	void DrawViewport(float DeltaTime);

	// ��Ⱦģ��
	void DrawMesh(float DeltaTime);

	// ��Ⱦ�ƹ�
	void DrawLight(float DeltaTime);

protected:
	map<int, FGeometry> Geometries;				// ������
	FDirectXDescriptorHeap DescriptorHeap;		// ������
	FConstantBufferViews MeshConstantBufferViews;			// ������������
	FConstantBufferViews MaterialConstantBufferViews;		// ���ʳ���������
	FConstantBufferViews LightConstantBufferViews;			// �ƹⳣ��������
	FConstantBufferViews ViewportConstantBufferViews;		// ���������������
};



