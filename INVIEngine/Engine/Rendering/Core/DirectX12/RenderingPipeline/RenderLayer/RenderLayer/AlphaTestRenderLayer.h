#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"


class FAlphaTestRenderLayer : public FRenderLayer
{
public:
	FAlphaTestRenderLayer();

	virtual void BuildShader() override;

	virtual int GetRenderLayerType() const override { return 2; }

	virtual void BuildPSO() override;

};

