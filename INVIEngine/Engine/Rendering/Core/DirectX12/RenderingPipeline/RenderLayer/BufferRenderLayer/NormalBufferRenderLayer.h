#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FNormalBufferRenderLayer : public FRenderLayer
{
public:
	FNormalBufferRenderLayer();
	~FNormalBufferRenderLayer() override;

	void Draw(float deltaTime) override;
	void BuildShader() override;
	void BuildPSO() override;
	void ResetPSO() override;
};

