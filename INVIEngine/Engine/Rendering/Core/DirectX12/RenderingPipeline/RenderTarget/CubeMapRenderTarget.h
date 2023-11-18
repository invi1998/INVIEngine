#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FCubeMapRenderTarget : public IDirectXDeviceInterface, public std::enable_shared_from_this<FCubeMapRenderTarget>
{
	friend class FDynamicCubeMap;

public:
	FCubeMapRenderTarget();

	void Init(UINT w, UINT h, const DXGI_FORMAT& format);

public:

	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetMap.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }

protected:
	void ResetViewport();
	void ResetScissorRect();

	void BuildRenderTarget();

	void BuildSRVDescriptor();
	void BuildRTVDescriptor();

private:
	UINT Width;		// �������
	UINT Height;	// �����߶�

	DXGI_FORMAT Format;				// ��Ⱦ�����ݸ�ʽ��RGBA)

	D3D12_VIEWPORT Viewport{};		// �ӿڲü��������Ϣ
	D3D12_RECT	ScissorRect{};		// ��������
									/*left�������������Ե�� x ���ꡣ
									top�����������ϱ�Ե�� y ���ꡣ
									right�����������ұ�Ե�� x ���ꡣ
									bottom�����������±�Ե�� y ���ꡣ*/

	ComPtr<ID3D12Resource> RenderTargetMap;		// ʵ�ʵ�D3D��Ⱦ��Դ

	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResourceView;		// shader��ԴCpu��ͼ SRV
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPUShaderResourceView;		// shader��ԴGpu��ͼ SRV
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> CPURenderTargetView{};		// ��ȾĿ����ͼ	RTV
};

