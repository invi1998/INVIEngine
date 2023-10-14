#pragma once
#include "Interface/DirectXDeviceInterface.h"

struct FStaticSampler : public IDirectXDeviceInterface_Struct
{
public:
	FStaticSampler();

	CD3DX12_STATIC_SAMPLER_DESC* GetData();

	int GetSize() const;

	void BuildStaticSampler();

private:
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDescriptors;

};

