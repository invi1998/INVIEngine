#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"


class GClientViewPort;

class FDynamicShadowMap:public FDynamicMap
{
public:
	FDynamicShadowMap();
	~FDynamicShadowMap() override;

	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;

public:
	void SetViewportPosition(const XMFLOAT3& position);
	void SetViewportRotation(const XMFLOAT3& rotation);
	void BuildViewPort(const XMFLOAT3& position);
	void BuildViewMatrix();

	// 构建深度模板描述
	void BuildDepthStencilViewDesc();
	void BuildShadowMapRenderTargetDescriptor();

protected:
	void BuildRenderTargetSRV();

protected:
	GClientViewPort* ShadowViewPort = nullptr;		// ShadowMap视口

};

