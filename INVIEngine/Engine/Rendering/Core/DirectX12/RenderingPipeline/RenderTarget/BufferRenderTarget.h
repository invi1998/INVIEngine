#pragma once
#include "Core/RenderTarget.h"

// 用于创建渲染目标的委托，用于在创建渲染目标时，将渲染目标的资源传递给外部
DEFINITION_SIMPLE_SINGLE_DELEGATE(FRenderTargetDelegate, void, ComPtr<ID3D12Resource>&);

class FBufferRenderTarget : public FRenderTarget
{
public:
	FRenderTargetDelegate OnRenderTargetCreated;	// 渲染目标创建委托

	FBufferRenderTarget();
	~FBufferRenderTarget() override;
	void Init(UINT w, UINT h, const DXGI_FORMAT& format) override;

	void BuildRenderTargetMap();

	void BuildRenderTarget() override;
	void BuildSRVDescriptor() override;
	void BuildRTVDescriptor() override;

	D3D12_CPU_DESCRIPTOR_HANDLE& GetCPURenderTargetView() { return RenderTargetViewCPU; }

private:
	D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetViewCPU;	// 渲染目标视图的CPU句柄
};

