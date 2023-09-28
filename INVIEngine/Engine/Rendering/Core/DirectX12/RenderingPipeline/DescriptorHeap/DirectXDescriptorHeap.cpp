#include "EngineMinimal.h"

#include "DirectXDescriptorHeap.h"

void FDirectXDescriptorHeap::Build(UINT InNumDescriptor)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 常量缓冲区构建

	// 堆描述
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = InNumDescriptor;		// 描述数量
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// 指定堆类型，常量缓冲区视图，着色器资源视图，无序访问视图组合类型描述符
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// 指定标记，着色器可见
	HeapDesc.NodeMask = 0;
	// 创建描述符堆
	GetD3dDevice()->CreateDescriptorHeap(
		&HeapDesc,
		IID_PPV_ARGS(&CBVHeap)
	);
}

void FDirectXDescriptorHeap::PreDraw(float delta_time)
{
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GetD3dGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
}
