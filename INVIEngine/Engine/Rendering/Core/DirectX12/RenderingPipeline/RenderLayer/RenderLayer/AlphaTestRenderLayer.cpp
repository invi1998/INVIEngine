#include "EngineMinimal.h"
#include "AlphaTestRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	RenderPriority = 2450;
	PipelineState = EPipelineState::AlphaTest;
}

void FAlphaTestRenderLayer::BuildShader()
{
	FRenderLayer::BuildShader();
}

int FAlphaTestRenderLayer::GetRenderLayerType() const
{
	return PipelineState;
}

void FAlphaTestRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(PipelineState);
}

void FAlphaTestRenderLayer::Draw(float deltaTime)
{
	// 渲染之前，重置PSO
	DirectXPipelineState->ResetPSO(PipelineState);

	FRenderLayer::Draw(deltaTime);
}
