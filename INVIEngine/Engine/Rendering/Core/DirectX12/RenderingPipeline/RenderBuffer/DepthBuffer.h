#pragma once
#include "Core/RenderBuffer.h"

extern std::shared_ptr<class FBufferRenderTarget> DepthBufferRenderTarget;

namespace DepthBuffer
{
	void BuildDepthBufferDescriptorHeap(const D3D12_CPU_DESCRIPTOR_HANDLE& CpuSRVDescHeap, const D3D12_GPU_DESCRIPTOR_HANDLE& GpuSRVDescHeap, int CBVDescSize, int offset);

	void CreateDepthBuffer(ID3D12Device* Device, ID3D12Resource* Resource);
}


