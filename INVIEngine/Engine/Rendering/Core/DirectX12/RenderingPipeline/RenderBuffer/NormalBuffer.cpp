#include "EngineMinimal.h"
#include "NormalBuffer.h"

FNormalBuffer::FNormalBuffer()
{
}

FNormalBuffer::~FNormalBuffer()
{
}

void FNormalBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FRenderBuffer::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FNormalBuffer::Init(int wid, int hei)
{
	FRenderBuffer::Init(wid, hei);
}

void FNormalBuffer::PreDraw(float DeltaTime)
{
	FRenderBuffer::PreDraw(DeltaTime);
}

void FNormalBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FRenderBuffer::UpdateCalculations(delta_time, viewport_info);
}

void FNormalBuffer::Build(const XMFLOAT3& center)
{
	FRenderBuffer::Build(center);
}

void FNormalBuffer::Draw(float deltaTime)
{
	FRenderBuffer::Draw(deltaTime);
}

void FNormalBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FNormalBuffer::BuildDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetRTV()
{
}

void FNormalBuffer::BuildSRVDescriptor()
{
}

void FNormalBuffer::BuildRTVDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetMap(ComPtr<ID3D12Resource>& OutResource)
{
}
