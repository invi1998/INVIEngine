#include "EngineMinimal.h"
#include "BufferRenderTarget.h"

FBufferRenderTarget::FBufferRenderTarget()
{
}

FBufferRenderTarget::~FBufferRenderTarget()
{
}

void FBufferRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	FRenderTarget::Init(w, h, format);
}

void FBufferRenderTarget::BuildRenderTarget()
{
	if (OnRenderTargetCreated.IsBound())
	{
		OnRenderTargetCreated.Execute(RenderTargetMap);
	}
}

void FBufferRenderTarget::BuildSRVDescriptor()
{
}

void FBufferRenderTarget::BuildRTVDescriptor()
{
	FRenderTarget::BuildRTVDescriptor();
}
