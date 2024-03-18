#include "EngineMinimal.h"
#include "DepthBuffer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

std::shared_ptr<class FBufferRenderTarget> DepthBufferRenderTarget;

void DepthBuffer::BuildDepthBufferDescriptorHeap(const D3D12_CPU_DESCRIPTOR_HANDLE& CpuSRVDescHeap,
	const D3D12_GPU_DESCRIPTOR_HANDLE& GpuSRVDescHeap, int CBVDescSize, int offset)
{
	DepthBufferRenderTarget->GetCPUShaderResourceView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(CpuSRVDescHeap, offset, CBVDescSize);
	DepthBufferRenderTarget->GetGPUShaderResourceView() = CD3DX12_GPU_DESCRIPTOR_HANDLE(GpuSRVDescHeap, offset, CBVDescSize);

	// DepthBufferRenderTarget->GetCPURenderTargetView()
}

void DepthBuffer::CreateDepthBuffer(ID3D12Device* Device, ID3D12Resource* Resource)
{
}
