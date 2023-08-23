#include "ConstantBufferViews.h"

#include "Core/Viewport/ViewportTransformation.h"
#include "Rendering/Core/RenderingResourcesUpdate.h"

void FConstantBufferViews::CreateConstant(UINT ObjectSize, UINT ObjectCount)
{
	Constants = std::make_shared<FRenderingResourcesUpdate>();
	Constants->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount);
}

void FConstantBufferViews::Update(int Index, const void* InData)
{
	Constants->Update(Index, InData);
}

void FConstantBufferViews::BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle, UINT constantBufferNum, UINT handleOffset)
{
	// 常量缓冲区

	// 通过驱动拿到当前描述符D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 计算描述符在描述符堆中应该存放位置的偏移
	D3D12_GPU_VIRTUAL_ADDRESS Addr = Constants->GetBuffer()->GetGPUVirtualAddress();

	for (UINT i = 0; i < constantBufferNum; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = InHandle;

		// 常量缓冲区描述
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
		CBVDesc.BufferLocation = Addr + i * Constants->GetConstantsBufferByteSize();		// 常量缓冲区地址
		CBVDesc.SizeInBytes = Constants->GetConstantsBufferByteSize();		// 获取常量缓冲区字节大小
		handle.Offset(i + handleOffset, DescriptorOffset);
		// 传入我们计算好偏移值的handle
		GetD3dDevice()->CreateConstantBufferView(&CBVDesc, handle);
	}
}
