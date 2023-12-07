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

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.h
