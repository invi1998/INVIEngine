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
	// 构造默认缓冲区
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


// 该继承可以实现
// 在当前类中保存一个弱指针，当我们把这个指针变成裸指针后，他可以将其变成一个智能指针（shared_ptr）
class FRenderingResourcesUpdate : public std::enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount);

	void Update(int Index, const void* InData);

	// 获取常量缓冲区字节大小(256的整数倍）
	UINT GetConstantsBufferByteSize(UINT InTypeSize);
	UINT GetConstantsBufferByteSize();

	ID3D12Resource* GetBuffer() const { return UploadBuffer.Get(); }

private:
	ComPtr<ID3D12Resource> UploadBuffer;		// 上传缓冲区
	UINT ElementSize;							// 元素大小
	BYTE* Data;									// 实际数据

};

