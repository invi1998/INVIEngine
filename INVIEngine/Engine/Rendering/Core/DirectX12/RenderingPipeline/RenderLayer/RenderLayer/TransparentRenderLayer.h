#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FTransparentRenderLayer : public FRenderLayer
{
public:
	FTransparentRenderLayer();

	virtual void BuildShader() override;

	virtual int GetRenderLayerType() const override { return 1; }

	virtual void BuildPSO() override;

	virtual void Draw(float deltaTime) override;
};

