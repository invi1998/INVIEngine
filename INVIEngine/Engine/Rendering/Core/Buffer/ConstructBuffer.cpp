#include "ConstructBuffer.h"


namespace ConstructBuffer
{

	ComPtr<ID3D12Resource> FConstructDefaultBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer,
		const void* InData, UINT64 InDataSize)
	{
			ComPtr<ID3D12Resource> Buffer;
			// ��Դ����
			CD3DX12_RESOURCE_DESC BufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);

			// ����Ĭ�϶�
			CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

			// �����ύ��Դ
			ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
				&BufferProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferResourceDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(Buffer.GetAddressOf())
			));

			// ��ʱ���ϴ�������
			CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			// �����ύ��Դ
			ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
				&UpdateBufferProperties,
				D3D12_HEAP_FLAG_NONE,
				&BufferResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,		// ���ÿɶ����õ�ǰ��Դ����Ⱦ��ˮ���пɶ�����Ϊ�����ʱ�ϴ���������������ˮ��������Ҫ��д���
				nullptr,
				IID_PPV_ARGS(OutTempBuffer.GetAddressOf())
			));

			// ��������Դ
			D3D12_SUBRESOURCE_DATA SubResourceData = {};
			SubResourceData.pData = InData;		// ��Դ����ָ�룬ָ������Ǹ���Դ��ַ
			SubResourceData.RowPitch = InDataSize;		// ��Դ�м�࣬��ȣ�Ҳ������Դ�������С���������ֽ�Ϊ��С��
			SubResourceData.SlicePitch = SubResourceData.RowPitch;		// ������Դ����ȼ�࣬Ҳ���ֽڴ�С

			// �����ԴΪ����Ŀ�� ����Ϊ������Ҫ���ϴ�����Դ���Ƶ�GPU�����Ը��Ƶ�Ŀ����Ҫָ����ǳ�����
			CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				Buffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,		// ��Դ��ǰ״̬
				D3D12_RESOURCE_STATE_COPY_DEST);	// ��ԴĿ��״̬ ָʾ��Դ�������ڴ渴�Ʋ�����Ŀ��

			// ִ����Դ״̬ת������ͬ��GPU
			GetD3dGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

			// ��������Դ�������
			// �����ݴ� CPU �ڴ渴�Ƶ� GPU �ڴ��е���Դ��ͨ������£�UpdateSubresources ���������������ϴ��ѣ�Upload Heap���еĶ���������������Լ�������Դ��Texture Resource����
			// UpdateSubresources<1> �� Direct3D 12 �е�һ������ģ�壬���ڽ���������Դ�е����ݴ� CPU �ڴ渴�Ƶ� GPU �ڴ��е���Դ��
			// �� UpdateSubresources ������ͬ��UpdateSubresources<1> ����ֱ�Ӳ�����������Դ��������Ҫ���� CopyBufferRegion()��CopyTextureRegion() ���� CopyResource() �Ⱥ�����
			UpdateSubresources<1>(
				GetD3dGraphicsCommandList().Get(),		// ָ��Ҫִ�е������б�
				Buffer.Get(),							// Ŀ����Դ����ָ��
				OutTempBuffer.Get(),					// �м䣨��ʱ����Դ����ָ��
				0,		// �м���Դ��ƫ���������ֽ�Ϊ��λ����
				0,		// ��Դ�е�һ������Դ�������� ��Чֵ��ΧΪ 0 �� MaxSubresources��
				1,		// ����Դ������С��<1>����ػ��У���ֵ�̶�Ϊ1 ��Դ�е�����Դ���� ��Чֵ��ΧΪ 1 �� (MaxSubresources - FirstSubresource) ��
				&SubResourceData		// ��Դ��ַ
			);

			// ��������Դ�󣬶���Դ����״̬ת����������״̬ת��Ϊ�ɶ�״̬
			CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
				Buffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_GENERIC_READ
			);

			return Buffer;
	}
}

