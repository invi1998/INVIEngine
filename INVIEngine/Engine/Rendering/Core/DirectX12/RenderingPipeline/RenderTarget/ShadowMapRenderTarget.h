#pragma once
#include "Core/RenderTarget.h"

class FShadowMapRenderTarget : public FRenderTarget
{
	friend class FDynamicShadowMap;
public:
	FShadowMapRenderTarget();

	void Init(UINT w, UINT h, const DXGI_FORMAT& format) override;

	void BuildRenderTarget() override;
	void BuildSRVDescriptor() override;
	void BuildDSVDescriptor() override;

protected:
	void BuildShadowConstantBuffer();

private:
	D3D12_CPU_DESCRIPTOR_HANDLE CPUDepthStencilView{};		// 深度模板视图	DSV

};

