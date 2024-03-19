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

#endif
