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

	// 构建模型
	void Build();

	UINT GetDrawObjectCount() const;

	// 获取顶点buff视图
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	// 获取顶点index缓冲区视图
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

	void DuplicateMesh(CMeshComponent* mesh_component, const FRenderingData& rendering_data, int geometryKey);
	bool FindMeshRenderingDataByHash(size_t hashKey, FRenderingData& rendering_data, int layer = -1);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU 顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU 索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU 顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU 索引缓冲区

	// 临时缓冲区，方便我们将CPU的buffer数据复制到GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// 临时 顶点缓冲区
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// 临时 索引缓冲区

	FMeshRenderingData MeshRenderingData;

	// std::vector<FRenderingData> DescribeMeshRenderingData;					// 渲染数据
};

// 几何模型映射阶段
struct FGeometryMap : IDirectXDeviceInterface_Struct
{
	friend class FRenderLayer;
	friend class FDynamicCubeMap;
public:
	FGeometryMap();
	~FGeometryMap();
	void BuildMesh(const size_t meshHash, CMeshComponent* Mesh, const FMeshRenderingData& MeshData);

	void Build();

	// 给描述堆分配堆栈内存
	void BuildDescriptorHeap();

	// 绘制贴图数量
	UINT GetDrawTexture2DCount() const;

	// 绘制CubeMap数量
	UINT GetDrawCubeMapCount() const;

	// 对象模型的绘制数量
	UINT GetDrawMeshCount();

	// 模型材质的绘制数量
	UINT GetDrawMaterialCount();

	// 灯光的绘制数量
	UINT GetDrawLightCount();

	// 构建对象模型的buff缓冲区
	void BuildMeshConstantBuffer();

	// 构建模型材质的buff缓冲区
	void BuildMaterialShaderResourceView();

	// 构建灯光的buff缓冲区
	void BuildLightConstantBuffer();

	// 构建雾的buff缓冲区
	void BuildFogConstantBuffer();

	void BuildFog();

	// 是否开启雾
	bool IsStartUpFog();

	// 构建视口buffer
	void BuildViewportConstantBuffer();

	// 渲染视口
	void DrawViewport(float DeltaTime);

	// 读取贴图
	void LoadTexture() const;

	// 构建贴图视图 SRV shader资源视图
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

	// 渲染模型
	void DrawMesh(float DeltaTime);

	// 绘制材质
	void DrawMaterial(float DeltaTime);

	// 渲染灯光
	void DrawLight(float DeltaTime);

	// 渲染纹理
	void DrawTexture(float DeltaTime);

	// 渲染雾
	void DrawFog(float DeltaTime);

protected:
	map<int, FGeometry> Geometries{};						// 几何体
	FDirectXDescriptorHeap DescriptorHeap;					// 描述堆
	FConstantBufferViews MeshConstantBufferViews;			// 对象常量缓冲区
	FConstantBufferViews MaterialConstantBufferViews;		// 材质常量缓冲区
	FConstantBufferViews LightConstantBufferViews;			// 灯光常量缓冲区
	FConstantBufferViews ViewportConstantBufferViews;		// 摄像机常量缓冲区
	FConstantBufferViews FogConstantBufferViews;			// 雾常量缓冲区

	std::shared_ptr<FRenderingTextureResourcesUpdate> RenderingTexture2DResourceViews;		// shader纹理资源视图
	std::shared_ptr<FRenderingTextureResourcesUpdate> RenderingCubeMapResourceViews;		// shader cube map资源视图
	std::vector<CMaterial*> MaterialsSRV{};

	CFogComponent* Fog;
};



