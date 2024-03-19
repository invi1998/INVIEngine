#include "EngineMinimal.h"
#include "DepthBuffer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

std::shared_ptr<class FBufferRenderTarget> DepthBufferRenderTarget = std::make_shared<FBufferRenderTarget>();

namespace DepthBuffer
{
	void BuildDepthBufferDescriptorHeap(const D3D12_CPU_DESCRIPTOR_HANDLE& CpuSRVDescHeap,
		const D3D12_GPU_DESCRIPTOR_HANDLE& GpuSRVDescHeap, int CBVDescSize, int offset)
	{
		DepthBufferRenderTarget->GetCPUShaderResourceView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(CpuSRVDescHeap, offset, CBVDescSize);
		DepthBufferRenderTarget->GetGPUShaderResourceView() = CD3DX12_GPU_DESCRIPTOR_HANDLE(GpuSRVDescHeap, offset, CBVDescSize);

		// DepthBufferRenderTarget->GetCPURenderTargetView()
	}

	void CreateDepthBuffer(ID3D12Device* Device, ID3D12Resource* Resource)
	{
		if (Device && Resource)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc{};
			ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;	// 默认映射
			ShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;	// 24位深度，8位模板
			ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		// 2D纹理
			ShaderResourceViewDesc.Texture2D.MipLevels = 1;		// MipMap等级
			ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;	// 最详细的MipMap等级
			
			Device->CreateShaderResourceView(Resource, &ShaderResourceViewDesc, DepthBufferRenderTarget->GetCPUShaderResourceView());
		}
	}
}

