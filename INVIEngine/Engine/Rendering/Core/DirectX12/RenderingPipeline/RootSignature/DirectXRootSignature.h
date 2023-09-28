#pragma once

#include "Interface/DirectXDeviceInterface.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXRootSignature();

	ID3D12RootSignature *GetRootSignature() const { return RootSignature.Get(); }

	void BuildRootSignature();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

private:
	ComPtr<ID3D12RootSignature> RootSignature;		// ¸ùÇ©Ãû
};

