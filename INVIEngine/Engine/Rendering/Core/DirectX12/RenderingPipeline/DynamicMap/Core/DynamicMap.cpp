#include "EngineMinimal.h"
#include "DynamicMap.h"

FDynamicMap::FDynamicMap() = default;

void FDynamicMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
	RenderLayers = inRenderLayer;
}

void FDynamicMap::PreDraw(float DeltaTime)
{
}

void FDynamicMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
}

void FDynamicMap::Build(const XMFLOAT3& center)
{
}

void FDynamicMap::Draw(float deltaTime)
{
}

void FDynamicMap::ResetView(int wid, int hei)
{
	Width = wid;
	Height = hei;
}
