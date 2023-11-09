#include "EngineMinimal.h"
#include "TransparantRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FTransparantRenderLayer::FTransparantRenderLayer()
{
	RenderPriority = 3000;
}

void FTransparantRenderLayer::BuildShader()
{
}

void FTransparantRenderLayer::BuildPSO()
{
	// 构建渲染管线
	DirectXPipelineState->BuildPipelineState(EPipelineState::Wireframe);
	DirectXPipelineState->SetFillModle(false);

	DirectXPipelineState->BuildPipelineState(EPipelineState::Solid);
}
