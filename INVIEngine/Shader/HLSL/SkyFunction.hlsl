#ifndef __SKY_FUNCTION__
#define __SKY_FUNCTION__

#include "ShaderCommon.hlsl"


float4 GetFogValue(float4 InColor, float3 InPointPosition)
{
#if START_UP_FOG
	float Distance = length(InPontPosition - CameraPosition.xyz);
	
	float AlphaValue = saturate((Distance - FogStart) / FogRange);
	
	InColor = lerp(InColor, FogColor, AlphaValue);
	
	return InColor;

#endif

	return InColor;

}

#endif
