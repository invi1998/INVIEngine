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
	// ����������

	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ����������������������Ӧ�ô��λ�õ�ƫ��
	D3D12_GPU_VIRTUAL_ADDRESS Addr = Constants->GetBuffer()->GetGPUVirtualAddress();

	for (UINT i = 0; i < constantBufferNum; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = InHandle;

		// ��������������
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
		CBVDesc.BufferLocation = Addr + i * Constants->GetConstantsBufferByteSize();		// ������������ַ
		CBVDesc.SizeInBytes = Constants->GetConstantsBufferByteSize();		// ��ȡ�����������ֽڴ�С
		handle.Offset(i + handleOffset, DescriptorOffset);
		// �������Ǽ����ƫ��ֵ��handle
		GetD3dDevice()->CreateConstantBufferView(&CBVDesc, handle);
	}
}
