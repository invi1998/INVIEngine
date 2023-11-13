#include "EngineMinimal.h"
#include "AlphaTestRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	RenderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{
	FRenderLayer::BuildShader();
}

void FAlphaTestRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(EPipelineState::AlphaTest);
}

void FAlphaTestRenderLayer::Draw(float deltaTime)
{
	// 渲染之前，重置PSO
	DirectXPipelineState->ResetPSO(1);

	FRenderLayer::Draw(deltaTime);
}
