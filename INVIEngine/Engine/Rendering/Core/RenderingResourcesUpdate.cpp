#include "EngineMinimal.h"

#include "RenderingResourcesUpdate.h"

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{
}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)
	{
		UploadBuffer->Unmap(0, nullptr);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount, bool bConstBuffer)
{
	// �����豸�����Ƿ����
	assert(InDevice);

	if (bConstBuffer)
	{
		ElementSize = GetConstantsBufferByteSize(InElementSize);;
	}
	else
	{
		ElementSize = InElementSize;
	}

	// ָ���ϴ���
	CD3DX12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ָ����Դ����
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElementCount);
	// �����ύ��Դ
	ANALYSIS_RESULT(InDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadBuffer)
	));

	// ����ӳ��
	ANALYSIS_RESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	// ͨ����ַƫ�ƣ���λ��ָ��������Ԫ�ص��׵�ַ��Ȼ��������Ҫ���µ����ݸ��ڴ濽���ķ�ʽ���������ǵ������У��������Ⱦ���Ԫ�ش�С
	memcpy(&Data[Index * ElementSize], InData, ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantsBufferByteSize(UINT InTypeSize)
{
	// ��δ������������㳣����������С�ģ����������ǽ������С����ȡ���� 256 �ֽڵ���������������˵����δ���ĺ������£�
	// ���ȣ�����Ҫ����Ŀռ��С���� 255��������Ϊ��Ҫ��������ȡ���������� 255 �� 256 �������������һ��
	// Ȼ�󣬽������ 0xFF00���� 1111 1111 0000 0000����λ�����㣬���� 8 λ���㣬������ȡ���� 256 �ֽڵ�������������ʹ���� ~255 ������ 0xFF00����Ϊ ~255 ��ʾ�� 255 ȡ�����õ��Ķ���������� 0xFF��
	// �ɴ˿ɼ�����δ���������ǽ���Ҫ����Ŀռ��С����ȡ���� 256 �ֽڵ�����������ʵ�ʱ���У����ּ��ɾ��������ڼ�����Դ��С���Ա�֤��Դ���뵽�ض��ı߽磬�Ӷ����Ч�ʲ�������ִ���
	return (InTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantsBufferByteSize()
{
	return GetConstantsBufferByteSize(ElementSize);
}

