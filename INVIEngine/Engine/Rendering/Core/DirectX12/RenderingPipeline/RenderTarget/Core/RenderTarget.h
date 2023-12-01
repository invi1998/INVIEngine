#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FRenderTarget : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderTarget>
{
public:
	FRenderTarget();
	virtual ~FRenderTarget();

	virtual void Init(UINT w, UINT h, const DXGI_FORMAT& format);
	virtual void ResetViewport();
	virtual void ResetScissorRect();
	virtual void BuildRenderTarget();
	virtual void BuildSRVDescriptor();
	virtual void BuildRTVDescriptor();

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.h
