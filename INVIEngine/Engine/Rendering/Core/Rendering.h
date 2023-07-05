#pragma once

#include "Core/Engine.h"
#include "Platform/Windows/WindowsEngine.h"

class IRenderingInterface
{
	friend class CWindowsEngine;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	inline simple_c_guid GetGuid() const { return Guid; }

	bool operator==(const IRenderingInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

protected:
	// ����Ĭ�ϻ�����
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize);

protected:

	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

	CWindowsEngine* GetEngine() const;

private:
	static std::vector<IRenderingInterface*> RenderingInterface;

	simple_c_guid Guid;

};


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

