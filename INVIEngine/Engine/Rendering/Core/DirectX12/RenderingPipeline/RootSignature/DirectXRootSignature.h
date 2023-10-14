#pragma once

#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/StaticSimpler/StaticSampler.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXRootSignature();

	ID3D12RootSignature *GetRootSignature() const { return RootSignature.Get(); }

	void BuildRootSignature(UINT textureNum = 1);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

private:
	ComPtr<ID3D12RootSignature> RootSignature;		// 根签名
	FStaticSampler StaticSamplerObject;				// 静态采样器
};

