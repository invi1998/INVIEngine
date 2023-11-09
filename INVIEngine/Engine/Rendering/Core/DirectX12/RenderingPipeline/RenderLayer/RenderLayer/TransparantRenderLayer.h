#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

class FTransparantRenderLayer : public FRenderLayer
{
public:
	FTransparantRenderLayer();

	virtual void BuildShader() override;

	virtual int GetRenderLayerType() const override { return 1; }

	virtual void BuildPSO() override;
};

