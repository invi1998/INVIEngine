#pragma once

#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXRootSignature();

	ID3D12RootSignature *GetRootSignature() const { return RootSignature.Get(); }

	void BuildRootSignature();

private:
	ComPtr<ID3D12RootSignature> RootSignature;		// ¸ùÇ©Ãû
};

