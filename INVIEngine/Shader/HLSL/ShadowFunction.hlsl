#ifndef __SHADOW_FUNCTION__
#define __SHADOW_FUNCTION__

#include "ShaderCommon.hlsl"

float GetShadowFactor(float4 worldPosition, float4x4 ShadowTransformMatrix)
{
	// 将世界坐标转换到阴影空间
	float4 ShadowPosition = mul(worldPosition, ShadowTransformMatrix);
	
	ShadowPosition.xyz /= ShadowPosition.w;
	
	ShadowPosition.xyz = ShadowPosition.xyz * 0.5f + 0.5f;
	
	float ShadowFactor = 1.f;
	
	if (ShadowPosition.x < 0.f || ShadowPosition.x > 1.f || ShadowPosition.y < 0.f || ShadowPosition.y > 1.f || ShadowPosition.z < 0.f || ShadowPosition.z > 1.f)
	{
		ShadowFactor = 0.f;
	}
	else
	{
		ShadowFactor = SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, ShadowPosition.xy, ShadowPosition.z);
	}
	
	return ShadowFactor;
}

#endif
