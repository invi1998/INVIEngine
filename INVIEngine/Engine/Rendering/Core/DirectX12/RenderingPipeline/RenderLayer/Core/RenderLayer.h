#pragma once
#include "Interface/DirectXDeviceInterface.h"
#include "Shader/Core/Shader.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/RenderingData.h"

struct FDirectXPipelineState;
struct FGeometryMap;

class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
public:
	FRenderLayer();
	virtual ~FRenderLayer() = default;

	void RegisterRenderLayer();

	const UINT GetRenderPriority() const { return RenderPriority; }


public:
	virtual void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	virtual void BuildShader() = 0;

protected:
	UINT RenderPriority;

	FShader VertexShader;
	FShader PixelShader;

	// 在 Direct3D 12 中，输入布局是一个包含多个输入元素的数据结构，表示顶点缓冲区中存储的顶点数据的格式和排列方式。
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// 描述输入布局（Input Layout）中的单个元素。

	std::vector<FRenderingData> RenderData;

	FGeometryMap* GeometryMap;
	FDirectXPipelineState* DirectXPipelineState;	// 管线状态 pso
};

