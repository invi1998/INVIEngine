#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FBackgroundRenderLayer : public FRenderLayer
{
public:
	FBackgroundRenderLayer();
	~FBackgroundRenderLayer() override;
	
	void PreDraw(float deltaTime) override;
	void Draw(float deltaTime) override;
	void PostDraw(float deltaTime) override;
	void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState) override;


	void BuildShader() override;
	void UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	void BuildPSO() override;
};

