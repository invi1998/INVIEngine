#ifndef __SHADOW_FUNCTION__
#define __SHADOW_FUNCTION__

#include "ShaderCommon.hlsl"

float GetShadowFactor(float4 worldPosition, float4x4 ShadowTransformMatrix)
{
	// 将世界坐标转换到阴影空间
	float4 ShadowPosition = mul(worldPosition, ShadowTransformMatrix);
	
	return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, ShadowPosition.xy, ShadowPosition.z);
}

#endif
