#pragma once

#include "EngineMinimal.h"


// �ü̳п���ʵ��
// �ڵ�ǰ���б���һ����ָ�룬�����ǰ����ָ������ָ��������Խ�����һ������ָ�루shared_ptr��
class FRenderingResourcesUpdate : public std::enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount);

	void Update(int Index, const void* InData);

	// ��ȡ�����������ֽڴ�С(256����������
	UINT GetConstantsBufferByteSize(UINT InTypeSize);
	UINT GetConstantsBufferByteSize();

	ID3D12Resource* GetBuffer() const { return UploadBuffer.Get(); }

private:
	ComPtr<ID3D12Resource> UploadBuffer;		// �ϴ�������
	UINT ElementSize;							// Ԫ�ش�С
	BYTE* Data;									// ʵ������

};

