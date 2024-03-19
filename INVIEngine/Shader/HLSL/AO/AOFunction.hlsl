#ifndef __AO_FUNCTION__
#define __AO_FUNCTION__

#include "AOCommon.hlsl"

// 将我们的深度值从NDC空间[-1, 1]转换到视口空间[0, 1]
float DepthNDCToView(float DepthNdc)
{
	float A = ProjectionMatrix[2][2];
	float B = ProjectionMatrix[3][2];
	
	return B / (DepthNdc - A);
}

// 遮蔽函数
float OcclusionFuncion(float DepthDistance)
{
	if (DepthDistance < ObscurationThreshold)
	{
		return 0.f;
	}
	
	float OcclusionLen = OcclusionFadeEnd - OcclusionFadeStart;
	return saturate((OcclusionFadeEnd - DepthDistance) / OcclusionLen);	// 不该有负值
}

#endif
