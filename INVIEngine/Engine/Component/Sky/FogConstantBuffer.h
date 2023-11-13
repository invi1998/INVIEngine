#pragma once
struct FFogConstantBuffer
{
	FFogConstantBuffer();

	XMFLOAT4 FogColor;

	float FogStart;
	float FogRange;
	float FogHeight;
	float XX1;
};

