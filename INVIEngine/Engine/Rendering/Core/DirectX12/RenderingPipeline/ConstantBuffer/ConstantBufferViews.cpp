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
	// ����������

	// ����������������������Ӧ�ô��λ�õ�ƫ��
	D3D12_GPU_VIRTUAL_ADDRESS Addr = Constants.get()->GetBuffer()->GetGPUVirtualAddress();
	
	// ͨ�������õ���ǰ������D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV��ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < constantBufferNum; i++)
	{
		handle.Offset(i + handleOffset, DescriptorOffset);

		// ��������������
		D3D12_CONSTANT_BUFFER_VIEW_DESC viewportCBVDes;
		viewportCBVDes.BufferLocation = Addr + Constants->GetConstantsBufferByteSize();		// ������������ַ
		viewportCBVDes.SizeInBytes = Constants->GetConstantsBufferByteSize();		// ��ȡ�����������ֽڴ�С
		// �������Ǽ����ƫ��ֵ��handle
		GetD3dDevice()->CreateConstantBufferView(&viewportCBVDes, handle);
	}
}
