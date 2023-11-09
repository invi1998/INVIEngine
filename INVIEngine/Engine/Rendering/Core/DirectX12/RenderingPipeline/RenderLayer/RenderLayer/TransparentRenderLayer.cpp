#include "EngineMinimal.h"
#include "TransparentRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	RenderPriority = 3000;
}

void FTransparentRenderLayer::BuildShader()
{

}

void FTransparentRenderLayer::BuildPSO()
{
	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(1);
}

void FTransparentRenderLayer::Draw(float deltaTime)
{
	// 渲染之前，重置PSO
	DirectXPipelineState->ResetPSO(0);

	FRenderLayer::Draw(deltaTime);

}
