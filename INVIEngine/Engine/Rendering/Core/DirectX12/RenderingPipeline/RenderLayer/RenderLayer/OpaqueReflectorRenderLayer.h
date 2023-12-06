#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"

// ≤ªÕ∏√˜∑¥…‰‰÷»æ≤„
class FOpaqueReflectorRenderLayer : public FRenderLayer
{
public:
	FOpaqueReflectorRenderLayer();

	virtual void BuildShader() override;

	virtual void BuildPSO() override;

	virtual void Draw(float deltaTime) override;

	void ResetPSO() override;

};

