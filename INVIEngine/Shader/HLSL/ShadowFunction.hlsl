#ifndef __SHADOW_FUNCTION__
#define __SHADOW_FUNCTION__

#include "ShaderCommon.hlsl"

float GetShadowFactor(float4 worldPosition, float4x4 ShadowTransformMatrix)
{
	// 将世界坐标转换到阴影空间
	float4 ShadowPosition = mul(worldPosition, ShadowTransformMatrix);
	
	return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, ShadowPosition.xy, ShadowPosition.z);
}

//直接采样
float GetShadowFactorByDirectSample(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);
	return SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy).r;
}

//经典的四个样本PCF采样 有模糊的锯齿
float GetShadowFactor_PCF_Sample4(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	uint Width = 0;
	uint Height = 0;
	uint NumMips = 0;

	SimpleShadowMap.GetDimensions(0, Width, Height, NumMips);

	float SampleSize = 1.f / Width;

	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);
	float S1 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy).r;
	float S2 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(SampleSize, 0.f)).r;
	float S3 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(0.f, SampleSize)).r;
	float S4 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(SampleSize, SampleSize)).r;

	ShadowPointHome.xyz /= ShadowPointHome.w;

	float ShadowDepth = ShadowPointHome.z;

	float R1 = ShadowDepth <= S1;
	float R2 = ShadowDepth <= S2;
	float R3 = ShadowDepth <= S3;
	float R4 = ShadowDepth <= S4;

	//转到纹素空间
	float2 TexelsPosition = frac(Width * ShadowPointHome.xy);

	return lerp(
		lerp(R1, R2, TexelsPosition.x),
		lerp(R3, R4, TexelsPosition.x),
		TexelsPosition.y);
}

//九个点的PCF采样
float GetShadowFactor_PCF_Sample9(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);

	ShadowPointHome.xyz /= ShadowPointHome.w;
	float ShadowDepth = ShadowPointHome.z;

	uint Width = 0;
	uint Height = 0;
	uint NumMips = 0;

	SimpleShadowMap.GetDimensions(0, Width, Height, NumMips);
	float SampleSize = 1.f / Width;

	const float2 ConstShadowSample[9] =
	{
		float2(-SampleSize, -SampleSize), float2(0.0f, -SampleSize), float2(SampleSize, -SampleSize),
		float2(-SampleSize, 0.0f), float2(0.0f, 0.0f), float2(SampleSize, 0.0f),
		float2(-SampleSize, +SampleSize), float2(0.0f, +SampleSize), float2(SampleSize, +SampleSize)
	};

	float R = 0.f;

	for (int i = 0; i < 9; i++)
	{
		R += SimpleShadowMap.SampleCmpLevelZero(
			ShadowSampler,
			ShadowPointHome.xy + ConstShadowSample[i],
			ShadowDepth).r;
	}

	// 当R为0时，会导致阴影细节丢失，变成存粹的黑色剪影
	return max(R / 9.f, 0.5f);
}

#endif
