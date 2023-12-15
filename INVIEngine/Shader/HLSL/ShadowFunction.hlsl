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


float GetShadowFactor_PCF(float4 worldPosition, float4x4 ShadowTransformMatrix)
{
	
	float W = 2048.f;
	float SampleSize = 1.f / W;
	
	// 将世界坐标转换到阴影空间
	float4 ShadowPosition = mul(worldPosition, ShadowTransformMatrix);
	
	return SimpleShadowMap.Sample(TextureSampler, ShadowPosition.xy).r;
	
}

#endif
