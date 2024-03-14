#pragma once
#include "DefaultDirectXRootSignature.h"

// SSAOµÄDirectX¸ùÇ©Ãû
class FSSAODirectXRootSignature : public FDirectXRootSignature
{
public:
	ID3D12RootSignature* GetRootSignature() const override;
	void BuildRootSignature(UINT textureNum) override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
};

