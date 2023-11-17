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
	UINT Width;		// 画布宽度
	UINT Height;	// 画布高度

	DXGI_FORMAT Format;				// 渲染的数据格式（RGBA)

	D3D12_VIEWPORT Viewport{};		// 视口裁剪的相关信息
	D3D12_RECT	ScissorRect{};		// 矩形区域
									/*left：矩形区域左边缘的 x 坐标。
									top：矩形区域上边缘的 y 坐标。
									right：矩形区域右边缘的 x 坐标。
									bottom：矩形区域下边缘的 y 坐标。*/

	ComPtr<ID3D12Resource> RenderTargetMap;		// 实际的D3D渲染资源
};

