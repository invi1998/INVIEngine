#include "EngineMinimal.h"
#include "RenderBuffer.h"

FRenderBuffer::FRenderBuffer()
{
}

FRenderBuffer::~FRenderBuffer()
{
}

void FRenderBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FRenderBuffer::Init(int wid, int hei)
{
	FDynamicMap::Init(wid, hei);
}

void FRenderBuffer::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FRenderBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FRenderBuffer::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FRenderBuffer::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);
}

void FRenderBuffer::ResetView(int wid, int hei)
{
	FDynamicMap::ResetView(wid, hei);
}
