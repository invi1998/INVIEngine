#include "EngineMinimal.h"
#include "RenderBuffer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

FRenderBuffer::FRenderBuffer()
{
	CreateRenderTarget<FBufferRenderTarget>();
}

FRenderBuffer::~FRenderBuffer()
{
}

void FRenderBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);

	// 绑定渲染目标创建事件
	if (FBufferRenderTarget* rendertarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		rendertarget->OnRenderTargetCreated.Bind(this, &FRenderBuffer::BuildRenderTargetBuffer);
	}
}

void FRenderBuffer::SetBufferSize(int wid, int hei)
{
	FDynamicMap::SetBufferSize(wid, hei);

	RenderTarget->Init(wid, hei, Format);
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

void FRenderBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
}
