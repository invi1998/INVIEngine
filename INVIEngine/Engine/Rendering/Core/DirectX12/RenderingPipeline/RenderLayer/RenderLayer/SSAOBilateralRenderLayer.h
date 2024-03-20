#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FSSAOBilateralRenderLayer : public FRenderLayer
{
public:
	FSSAOBilateralRenderLayer();

	virtual void BuildShader() override;

	virtual void BuildPSO() override;

	virtual void Draw(float deltaTime) override;

	void ResetPSO() override;
};

