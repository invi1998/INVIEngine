#pragma once

#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/StaticSimpler/StaticSampler.h"

struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXRootSignature();

	virtual ID3D12RootSignature *GetRootSignature() const { return RootSignature.Get(); }

	virtual void BuildRootSignature(UINT textureNum = 1);

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	FStaticSampler& GetStaticSampler() { return StaticSamplerObject; }
	ComPtr<ID3D12RootSignature>& GetRootSignatureComPtr() { return RootSignature; }

protected:
	ComPtr<ID3D12RootSignature> RootSignature;		// 根签名
	FStaticSampler StaticSamplerObject;				// 静态采样器
};

