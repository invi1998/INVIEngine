#include "EngineMinimal.h"
#include "BackgroundRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	RenderPriority = 1000;
	PipelineState = EPipelineState::Background;
}

FBackgroundRenderLayer::~FBackgroundRenderLayer()
{
}

int FBackgroundRenderLayer::GetRenderLayerType() const
{
	return PipelineState;
}

void FBackgroundRenderLayer::PreDraw(float deltaTime)
{
	FRenderLayer::PreDraw(deltaTime);
}

void FBackgroundRenderLayer::Draw(float deltaTime)
{
	FRenderLayer::Draw(deltaTime);

	DirectXPipelineState->ResetPSO(PipelineState);
}

void FBackgroundRenderLayer::PostDraw(float deltaTime)
{
	FRenderLayer::PostDraw(deltaTime);
}

void FBackgroundRenderLayer::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	FRenderLayer::Init(geometry, directXPipelineState);
}

void FBackgroundRenderLayer::BuildShader()
{
	FRenderLayer::BuildShader();
}

void FBackgroundRenderLayer::UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FRenderLayer::UpdateCaculations(DeltaTime, ViewportInfo);
}

void FBackgroundRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	DirectXPipelineState->BuildPipelineState(EPipelineState::Background);

	
}
