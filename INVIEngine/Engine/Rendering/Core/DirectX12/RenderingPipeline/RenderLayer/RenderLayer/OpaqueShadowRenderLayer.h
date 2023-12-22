#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FOpaqueShadowRenderLayer : public FRenderLayer
{
public:
	FOpaqueShadowRenderLayer();
	~FOpaqueShadowRenderLayer() override;

	void PreDraw(float deltaTime) override;
	void Draw(float deltaTime) override;
	void PostDraw(float deltaTime) override;

	void BuildShader() override;
	void BuildPSO() override;

	void ResetPSO() override;

	void ResetPSO(EPipelineState ps) override;

	void DrawMesh(float DeltaTime, ERenderCondition rc = RC_Always) override;

	// 构建全向阴影贴图shader
	void BuildViewtianeShadowShader();
};
// 
