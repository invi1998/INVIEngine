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
	// ������Ⱦ����
	DirectXPipelineState->BuildPipelineState(1);
}

void FTransparentRenderLayer::Draw(float deltaTime)
{
	// ��Ⱦ֮ǰ������PSO
	DirectXPipelineState->ResetPSO(0);

	FRenderLayer::Draw(deltaTime);

}
