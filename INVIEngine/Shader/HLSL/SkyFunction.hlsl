#ifndef __SKY_FUNCTION__
#define __SKY_FUNCTION__

#include "ShaderCommon.hlsl"


float4 GetFogValue(float4 InColor, float3 InPointPosition)
{
#if START_UP_FOG
	float Distance = length(InPointPosition - CameraPosition.xyz);
	
	float AlphaValue = saturate((Distance - FogStart) / FogRange);
	
	float4 Color = lerp(InColor, FogColor, AlphaValue);
	
	float HeightAlphaValue = saturate((InPointPosition.y - CameraPosition.y) / max(FogHeight, 1.f));
	
	InColor = lerp(Color, InColor, HeightAlphaValue);
	

#endif

	return InColor;

}

#endif
