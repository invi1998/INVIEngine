#include "EngineMinimal.h"
#include "DynamicShadowMap.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
{
	CreateRenderTarget<FShadowMapRenderTarget>();
}

FDynamicShadowMap::~FDynamicShadowMap()
{
}

void FDynamicShadowMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FDynamicShadowMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicShadowMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);
}
