#pragma once

#include "MeshType.h"
#include "Core/Engine.h"
#include "Rendering/Core/Rendering.h"
#include "Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;
};

class FMesh : public IRenderingInterface
{
public:
	FMesh();
	~FMesh() override;

	virtual void Init() override;

	virtual void Draw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRendingData* InRenderingData);

	static FMesh* CreateMesh(const FMeshRendingData* InRenderingData);

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

	ComPtr<ID3D12RootSignature> RootSignature;		// 根签名
	ComPtr<ID3D12PipelineState> PSO;				// 渲染管线状态对象

	ComPtr<ID3D12DescriptorHeap> CBVHeap;			// CPU 描述符句柄，用于指定要创建的常量缓冲区视图所在的描述符堆位置

	std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	// 对象常量（指向上传堆）

	FShader VertexShader;	// 顶点着色器
	FShader PixelShader;	// 像素着色器

	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// 描述输入布局（Input Layout）中的单个元素。在 Direct3D 12 中，输入布局是一个包含多个输入元素的数据结构，表示顶点缓冲区中存储的顶点数据的格式和排列方式。

protected:
	UINT IndexSize;				// 顶点数量

	UINT VertexSizeInBytes;		// 顶点数据字节大小
	UINT VertexStrideInBytes;	// 顶点数据字节跨度(单位顶点数据大小）

	UINT IndexSizeInBytes;		// 顶点数据字节大小
	DXGI_FORMAT IndexFormat;	// 顶点索引数据格式（无符号16）

};

