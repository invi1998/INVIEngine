#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"


class FSelectRenderLayer : public FRenderLayer
{
public:
	FSelectRenderLayer();

	void BuildShader() override;

	void BuildPSO() override;

	void Draw(float deltaTime) override;

	void ResetPSO() override;
};

