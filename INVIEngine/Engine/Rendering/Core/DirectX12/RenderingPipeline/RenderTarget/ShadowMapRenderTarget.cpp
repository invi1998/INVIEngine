#include "EngineMinimal.h"
#include "ShadowMapRenderTarget.h"

FShadowMapRenderTarget::FShadowMapRenderTarget() : FRenderTarget()
{
}

void FShadowMapRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	FRenderTarget::Init(w, h, format);
}

void FShadowMapRenderTarget::BuildRenderTarget()
{
}

void FShadowMapRenderTarget::BuildSRVDescriptor()
{
}

void FShadowMapRenderTarget::BuildRTVDescriptor()
{
}
