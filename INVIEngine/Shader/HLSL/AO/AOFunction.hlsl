#ifndef __AO_FUNCTION__
#define __AO_FUNCTION__

#include "AOCommon.hlsl"

// �����ǵ����ֵ��NDC�ռ�[-1, 1]ת�����ӿڿռ�[0, 1]
float DepthNDCToView(float DepthNdc)
{
	float A = ProjectionMatrix[2][2];
	float B = ProjectionMatrix[3][2];
	
	return B / (DepthNdc - A);
}

// �ڱκ���
float OcclusionFuncion(float DepthDistance)
{
	if (DepthDistance < ObscurationThreshold)
	{
		return 0.f;
	}
	
	float OcclusionLen = OcclusionFadeEnd - OcclusionFadeStart;
	return saturate((OcclusionFadeEnd - DepthDistance) / OcclusionLen);	// �����и�ֵ
}

#endif
