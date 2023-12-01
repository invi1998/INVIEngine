#pragma once
#include "Core/RenderTarget.h"

class FCubeMapRenderTarget : public FRenderTarget
{
	friend class FDynamicCubeMap;

public:
	FCubeMapRenderTarget();

	void Init(UINT w, UINT h, const DXGI_FORMAT& format) override;

protected:

	void BuildRenderTarget() override;
	void BuildSRVDescriptor() override;
	void BuildRTVDescriptor() override;

private:
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> CPURenderTargetView{};		// ��ȾĿ����ͼ	RTV
};

