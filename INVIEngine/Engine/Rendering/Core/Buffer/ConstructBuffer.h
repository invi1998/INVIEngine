#pragma once

#include "Core/Engine.h"
#include "Interface/DirectXDeviceInterface.h"

namespace ConstructBuffer
{
	struct FConstructDefaultBuffer : public IDirectXDeviceInterface_Struct
	{
		// ����Ĭ�ϻ�����
		ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize);
	};
};

