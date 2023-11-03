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
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;

	std::vector<FRenderingData> RenderData;

	FGeometryMap* Geometry;
	FDirectXPipelineState* DirectXPipelineState;
};

