#pragma once

#include "EngineMinimal.h"
#include "RenderingData.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/MeshType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/ConstantBuffer/ConstantBufferViews.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

struct FViewportInfo;

struct FGeometry : IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;
public:
	bool bRenderingDataExistence(GMesh* InKey);
	void BuildMesh(GMesh* Mesh, const FMeshRenderingData& MeshData);

	// 构建模型
	void Build();

	UINT GetDrawObjectCount() const;

	// 获取顶点buff视图
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	// 获取顶点index缓冲区视图
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU 顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU 索引缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU 顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU 索引缓冲区

	// 临时缓冲区，方便我们将CPU的buffer数据复制到GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// 临时 顶点缓冲区
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// 临时 索引缓冲区

	FMeshRenderingData MeshRenderingData;

	std::vector<FRenderingData> DescribeMeshRenderingData;					// 渲染数据
};

// 几何模型映射阶段
struct FGeometryMap : IDirectXDeviceInterface_Struct
{
public:
	FGeometryMap();
	void BuildMesh(GMesh* Mesh, const FMeshRenderingData& MeshData);

	void Build();

	void BuildDescriptorHeap();

	UINT GetDrawObjectCount();

	void BuildConstantBuffer();

	void BuildViewportConstantBuffer();

	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	ID3D12DescriptorHeap* GetHeap() const { return DescriptorHeap.GetHeap(); }

private:
	// 渲染视口
	void DrawViewport(float DeltaTime);

	// 渲染模型
	void DrawMesh(float DeltaTime);

protected:
	map<int, FGeometry> Geometries;				// 几何体
	FDirectXDescriptorHeap DescriptorHeap;		// 描述堆
	FConstantBufferViews ObjectConstantBufferViews;		// 对象常量缓冲区
	FConstantBufferViews ViewportConstantBufferViews;	// 摄像机常量缓冲区
};



