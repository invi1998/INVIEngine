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

void FBufferRenderTarget::BuildRenderTargetMap()
{
	if (OnRenderTargetCreated.IsBound())
	{
		ComPtr<ID3D12Resource> rendertarget = GetRenderTarget();
		OnRenderTargetCreated.Execute(rendertarget);
	}
}

void FBufferRenderTarget::BuildRenderTarget()
{
	
}

void FBufferRenderTarget::BuildSRVDescriptor()
{
}

void FBufferRenderTarget::BuildRTVDescriptor()
{
	FRenderTarget::BuildRTVDescriptor();
}
