#pragma once

#include "Core/Engine.h"

class IRenderingInterface
{
	friend class FWindowsEngine;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();
	virtual void Draw(float DeltaTime);

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

private:
	static std::vector<IRenderingInterface*> RenderingInterface;

	simple_c_guid Guid;

};

