#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"


class FOpaqueRenderLayer : public FRenderLayer
{
public:
	FOpaqueRenderLayer();

	virtual void BuildShader() override;

	virtual int GetRenderLayerType() const override { return 0; }

	virtual void BuildPSO() override;

};

