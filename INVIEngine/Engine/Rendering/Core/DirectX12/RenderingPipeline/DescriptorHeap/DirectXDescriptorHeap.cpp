#include "EngineMinimal.h"

#include "DirectXDescriptorHeap.h"

void FDirectXDescriptorHeap::Build(UINT InNumDescriptor)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��������������

	// ������
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = InNumDescriptor;		// ��������
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;	// ָ�������ͣ�������������ͼ����ɫ����Դ��ͼ�����������ͼ�������������
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// ָ����ǣ���ɫ���ɼ�
	HeapDesc.NodeMask = 0;
	// ������������
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
