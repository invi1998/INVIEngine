#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FRenderTarget : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderTarget>
{
public:
	FRenderTarget();
	~FRenderTarget() override = default;

	virtual void Init(UINT w, UINT h, const DXGI_FORMAT& format);
	virtual void ResetViewport();
	virtual void ResetScissorRect();

	virtual void BuildRenderTarget() = 0;
	virtual void BuildSRVDescriptor() = 0;

	virtual void BuildRTVDescriptor() {};
	virtual void BuildDSVDescriptor() {};

public:

	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetMap.Get(); }
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	FORCEINLINE D3D12_RECT GetScissorRect() const { return ScissorRect; }
	FORCEINLINE CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUShaderResourceView() const { return GPUShaderResourceView; }
	FORCEINLINE CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUShaderResourceView() const { return CPUShaderResourceView; }

protected:
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
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResourceView;		// shader资源Cpu视图 SRV
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPUShaderResourceView;		// shader资源Gpu视图 SRV

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.h
