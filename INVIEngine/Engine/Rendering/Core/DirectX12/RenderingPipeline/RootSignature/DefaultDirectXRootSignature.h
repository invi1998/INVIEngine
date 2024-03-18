#pragma once
#include "Core/DirectXRootSignature.h"

// 默认的DirectX根签名
class FDefaultDirectXRootSignature : public FDirectXRootSignature
{
public:
	FDefaultDirectXRootSignature();

	void BuildRootSignature(UINT textureNum = 1) override;

	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;

};

