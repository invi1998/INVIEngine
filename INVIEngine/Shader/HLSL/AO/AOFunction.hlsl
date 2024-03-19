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

#endif
