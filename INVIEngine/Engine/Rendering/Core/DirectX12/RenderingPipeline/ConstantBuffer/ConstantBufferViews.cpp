#include "ConstantBufferViews.h"

#include "Core/Viewport/ViewportTransformation.h"
#include "Rendering/Core/RenderingResourcesUpdate.h"

void FConstantBufferViews::CreateConstant(UINT ObjectSize, UINT ObjectCount)
{
	Constants = std::make_shared<FRenderingResourcesUpdate>();
	Constants->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount);
}

void FConstantBufferViews::BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE handle, UINT constantBufferNum, UINT handleOffset)
{
	// 常量缓冲区

	// 计算描述符在描述符堆中应该存放位置的偏移
	D3D12_GPU_VIRTUAL_ADDRESS Addr = Constants.get()->GetBuffer()->GetGPUVirtualAddress();
	
	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < constantBufferNum; i++)
	{
		handle.Offset(i + handleOffset, DescriptorOffset);

		// 常量缓冲区描述
		D3D12_CONSTANT_BUFFER_VIEW_DESC viewportCBVDes;
		viewportCBVDes.BufferLocation = Addr + Constants->GetConstantsBufferByteSize();		// 常量缓冲区地址
		viewportCBVDes.SizeInBytes = Constants->GetConstantsBufferByteSize();		// 获取常量缓冲区字节大小
		// 传入我们计算好偏移值的handle
		GetD3dDevice()->CreateConstantBufferView(&viewportCBVDes, handle);
	}
}
