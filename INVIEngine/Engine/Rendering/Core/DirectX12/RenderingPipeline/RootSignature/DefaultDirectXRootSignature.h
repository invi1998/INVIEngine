#pragma once
#include "Core/DirectXRootSignature.h"

// Ĭ�ϵ�DirectX��ǩ��
class FDefaultDirectXRootSignature : public FDirectXRootSignature
{
public:
	FDefaultDirectXRootSignature();

	void BuildRootSignature(UINT textureNum = 1) override;

	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;

};

