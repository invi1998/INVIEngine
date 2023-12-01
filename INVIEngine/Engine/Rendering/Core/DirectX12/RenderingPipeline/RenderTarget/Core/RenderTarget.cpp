#include "EngineMinimal.h"
#include "RenderTarget.h"

FRenderTarget::FRenderTarget(): Width(256), Height(256), Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	FRenderTarget::ResetViewport();

	FRenderTarget::ResetScissorRect();
}

void FRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	Width = w;
	Height = h;
	Format = format;

	ResetViewport();

	ResetScissorRect();

	BuildRenderTarget();

	BuildSRVDescriptor();

	BuildRTVDescriptor();
}

void FRenderTarget::ResetViewport()
{
	Viewport = {
		0.f,
		0.f,
		static_cast<float>(Width),
		static_cast<float>(Height),
		0.f,	// 最小深度
		1.f		// 最大深度
	};
}

void FRenderTarget::ResetScissorRect()
{
	ScissorRect = {
		0,
		0,
		static_cast<LONG>(Width),
		static_cast<LONG>(Height)
	};
}

