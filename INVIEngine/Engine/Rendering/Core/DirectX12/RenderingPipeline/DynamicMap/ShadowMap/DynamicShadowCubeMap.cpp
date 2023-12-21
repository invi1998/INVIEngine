#include "EngineMinimal.h"
#include "DynamicShadowCubeMap.h"

FDynamicShadowCubeMap::FDynamicShadowCubeMap()
{
}

FDynamicShadowCubeMap::~FDynamicShadowCubeMap()
{
}

void FDynamicShadowCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowCubeMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FDynamicShadowCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicShadowCubeMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowCubeMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);
}

void FDynamicShadowCubeMap::ResetView(int wid, int hei)
{
	FDynamicMap::ResetView(wid, hei);
}
