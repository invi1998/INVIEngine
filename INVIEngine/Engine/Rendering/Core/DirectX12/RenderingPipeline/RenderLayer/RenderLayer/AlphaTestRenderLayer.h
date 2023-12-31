#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"


class FAlphaTestRenderLayer : public FRenderLayer
{
public:
	FAlphaTestRenderLayer();

	virtual void BuildShader() override;

	virtual void BuildPSO() override;

	virtual void Draw(float deltaTime) override;

	void ResetPSO() override;

};

