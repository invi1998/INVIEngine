#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FOperationHandleRenderLayer : public FRenderLayer
{
public:
	FOperationHandleRenderLayer();

	void BuildShader() override;

	void BuildPSO() override;

	void Draw(float deltaTime) override;

	void ResetPSO() override;
};

