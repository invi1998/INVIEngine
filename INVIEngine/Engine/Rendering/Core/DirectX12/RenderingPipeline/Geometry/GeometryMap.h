#pragma once

#include "RenderingData.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/MeshType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/ConstantBuffer/ConstantBufferViews.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

class CFogComponent;
struct FRenderingTexture;
class CMaterial;
class FRenderingTextureResourcesUpdate;
class CMeshComponent;
struct FViewportInfo;

struct FGeometry : IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;
public:
	bool bRenderingDataExistence(CMeshComponent* InKey);
	void BuildMesh(const size_t meshHash, CMeshComponent* Mesh, const FMeshRenderingData& MeshData, int geometryKey);

	// ����ģ��
	void Build();

	UINT GetDrawObjectCount() const;

	// ��ȡ����buff��ͼ
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	// ��ȡ����index��������ͼ
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

	void DuplicateMesh(CMeshComponent* mesh_component, const FRenderingData& rendering_data, int geometryKey);
	bool FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer = -1);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU ���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU ����������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU ���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU ����������

	// ��ʱ���������������ǽ�CPU��buffer���ݸ��Ƶ�GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// ��ʱ ���㻺����
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// ��ʱ ����������

	FMeshRenderingData MeshRenderingData;

	// std::vector<FRenderingData> DescribeMeshRenderingData;					// ��Ⱦ����
};

// ����ģ��ӳ��׶�
struct FGeometryMap : IDirectXDeviceInterface_Struct
{
	friend class FRenderLayer;
	friend class FDynamicCubeMap;
public:
	FGeometryMap();
	~FGeometryMap();
	void BuildMesh(const size_t meshHash, CMeshComponent* Mesh, const FMeshRenderingData& MeshData);

	void Build();

	// �������ѷ����ջ�ڴ�
	void BuildDescriptorHeap();

	// ������ͼ����
	UINT GetDrawTexture2DCount() const;

	// ����CubeMap����
	UINT GetDrawCubeMapCount() const;

	// ����ģ�͵Ļ�������
	UINT GetDrawMeshCount();

	// ģ�Ͳ��ʵĻ�������
	UINT GetDrawMaterialCount();

	// �ƹ�Ļ�������
	UINT GetDrawLightCount();

	// ��������ģ�͵�buff������
	void BuildMeshConstantBuffer();

	// ����ģ�Ͳ��ʵ�buff������
	void BuildMaterialShaderResourceView();

	// �����ƹ��buff������
	void BuildLightConstantBuffer();

	// �������buff������
	void BuildFogConstantBuffer();

	void BuildFog();

	// �Ƿ�����
	bool IsStartUpFog();

	// �����ӿ�buffer
	void BuildViewportConstantBuffer();

	// ��Ⱦ�ӿ�
	void DrawViewport(float DeltaTime);

	// ��ȡ��ͼ
	void LoadTexture() const;

	// ������ͼ��ͼ SRV shader��Դ��ͼ
	void BuildTextureConstBuffer();

	void UpdateCalculationViewport(const FViewportInfo& viewport_info, UINT constBufferOffset);

	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

	void UpdateMaterialShaderResourceView(float delta_time, const FViewportInfo& viewport_info);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	ID3D12DescriptorHeap* GetHeap() const { return DescriptorHeap.GetHeap(); }
	void DuplicateMesh(CMeshComponent* mesh_component, const FRenderingData& rendering_data);
	bool FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer = -1);

	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& key);

private:

	// ��Ⱦģ��
	void DrawMesh(float DeltaTime);

	// ���Ʋ���
	void DrawMaterial(float DeltaTime);

	// ��Ⱦ�ƹ�
	void DrawLight(float DeltaTime);

	// ��Ⱦ����
	void DrawTexture(float DeltaTime);

	// ��Ⱦ��
	void DrawFog(float DeltaTime);

protected:
	map<int, FGeometry> Geometries{};						// ������
	FDirectXDescriptorHeap DescriptorHeap;					// ������
	FConstantBufferViews MeshConstantBufferViews;			// ������������
	FConstantBufferViews MaterialConstantBufferViews;		// ���ʳ���������
	FConstantBufferViews LightConstantBufferViews;			// �ƹⳣ��������
	FConstantBufferViews ViewportConstantBufferViews;		// ���������������
	FConstantBufferViews FogConstantBufferViews;			// ����������

	std::shared_ptr<FRenderingTextureResourcesUpdate> RenderingTexture2DResourceViews;		// shader������Դ��ͼ
	std::shared_ptr<FRenderingTextureResourcesUpdate> RenderingCubeMapResourceViews;		// shader cube map��Դ��ͼ
	std::vector<CMaterial*> MaterialsSRV{};

	CFogComponent* Fog;
};



