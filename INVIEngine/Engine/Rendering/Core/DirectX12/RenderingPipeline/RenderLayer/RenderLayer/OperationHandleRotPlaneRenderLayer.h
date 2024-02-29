#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FOperationHandleRotPlaneRenderLayer : public FRenderLayer
{
public:
	FOperationHandleRotPlaneRenderLayer();

	void BuildShader() override;

	void BuildPSO() override;

	void Draw(float deltaTime) override;

	void ResetPSO() override;
};

