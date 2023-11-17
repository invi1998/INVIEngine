#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FCubeMapRenderTarget : public IDirectXDeviceInterface, public std::enable_shared_from_this<FCubeMapRenderTarget>
{
public:
	FCubeMapRenderTarget();

	void Init(UINT w, UINT h, const DXGI_FORMAT& format);

	void ResetViewport();
	void ResetScissorRect();

	void BuildRenderTarget();

public:

	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetMap.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }

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
};

