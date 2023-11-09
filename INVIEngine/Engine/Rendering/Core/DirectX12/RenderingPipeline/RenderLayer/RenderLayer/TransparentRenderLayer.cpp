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
	DirectXPipelineState->BuildPipelineState(EPipelineState::Wireframe);
	DirectXPipelineState->SetFillModle(false);

	DirectXPipelineState->BuildPipelineState(EPipelineState::Solid);
}
