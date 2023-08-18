#pragma once

#include "EngineMinimal.h"
#include "Geometry/GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "Shader/Core/Shader.h"

struct FViewportInfo;

// 渲染流水线
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData);
	void BuildPipeline();
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

protected:
	FGeometryMap GeometryMap;
	
	FShader VertexShader;	// 顶点着色器
	FShader PixelShader;	// 像素着色器

	// 在 Direct3D 12 中，输入布局是一个包含多个输入元素的数据结构，表示顶点缓冲区中存储的顶点数据的格式和排列方式。
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// 描述输入布局（Input Layout）中的单个元素。

	FDirectXPipelineState DirectXPipelineState;		// 管线状态 pso
	FDirectXRootSignature DirectXRootSignature;		// 根签名
};

