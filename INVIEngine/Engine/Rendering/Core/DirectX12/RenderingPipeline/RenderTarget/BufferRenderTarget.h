#pragma once
#include "Core/RenderTarget.h"

// ���ڴ�����ȾĿ���ί�У������ڴ�����ȾĿ��ʱ������ȾĿ�����Դ���ݸ��ⲿ
DEFINITION_SIMPLE_SINGLE_DELEGATE(FRenderTargetDelegate, void, ComPtr<ID3D12Resource>&);

class FBufferRenderTarget : public FRenderTarget
{
public:
	FRenderTargetDelegate OnRenderTargetCreated;	// ��ȾĿ�괴��ί��

	FBufferRenderTarget();
	~FBufferRenderTarget() override;
	void Init(UINT w, UINT h, const DXGI_FORMAT& format) override;

	void BuildRenderTarget() override;
	void BuildSRVDescriptor() override;
	void BuildRTVDescriptor() override;

};

