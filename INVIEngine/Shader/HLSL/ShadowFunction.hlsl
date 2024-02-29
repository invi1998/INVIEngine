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


// 采样万向阴影贴图（点光源阴影贴图）直接对CubeMap采样
float ProcessingOmnidirectinalSampleCubeMapShadow(float3 position, float3 lightPosition)
{
	float3 direction = position - lightPosition;
	float shadowFactor = SimpleShadowCubeMap.Sample(TextureSampler, direction).r;
	return shadowFactor;
}

float ProcessingOmnidirectinalSampleCmpLevelZeroShadow(float3 position, float3 lightPosition)
{
	float3 direction = position - lightPosition;
	float depth = 0.f;
	float shadowFactor = SimpleShadowCubeMap.SampleCmpLevelZero(ShadowSampler, float4(direction, 1.f), depth).r;
	return shadowFactor;
}

//
float3 GetPointSphericalCoordinates(float3 InPoint)
{
	float AnglePre = (180.f / 3.1415926f);

	float R = sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y + InPoint.z * InPoint.z);
	float Thata = acos(InPoint.y / R) * AnglePre;
	float Fai = atan2(InPoint.z, InPoint.x) * AnglePre;

	return float3(R, Thata, Fai);
}

bool IsAngleRange(float InAngle, float X, float Y)
{
	if (InAngle >= min(X, Y))
	{
		if (InAngle <= max(X, Y))
		{
			return true;
		}
	}

	return false;
}

bool IsAngleAxisRange(
    float InAngle,
    float X,
    float Y,
    float InCriticalValue,
    bool bComMin)
{
	if (bComMin) //比小
	{
		if (InAngle > InCriticalValue)
		{
			if (InAngle <= max(X, Y))
			{
				return true;
			}
		}
		else if (InAngle < InCriticalValue)
		{
			if (InAngle >= min(X, Y))
			{
				return true;
			}
		}
		else if (InAngle == 0.f)
		{
			return true;
		}
	}
	else
	{
		if (InAngle > 0.f) //+
		{
			if (InAngle >= max(X, Y))
			{
				if (InAngle <= InCriticalValue)
				{
					return true;
				}
			}
		}
		else
		{
			if (InAngle <= min(X, Y))
			{
				if (InAngle >= -InCriticalValue)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool IsPointInCubeMapVieport(
    float InPointTheta,
    float InPointFai,
    float4 InCompareValue,
    int bAxisRange,
    float InCriticalValue,
    bool bComMin)
{
	if (IsAngleRange(InPointTheta, InCompareValue.x, InCompareValue.y))
	{
		if (bAxisRange == 0)
		{
			if (IsAngleAxisRange(InPointFai, InCompareValue.z, InCompareValue.w, InCriticalValue, bComMin))
			{
				return true;
			}
		}
		else if (bAxisRange == 1)
		{
			if (InPointFai >= InCompareValue.z)
			{
				if (InPointFai <= InCompareValue.w)
				{
					return true;
				}
			}
		}
		else if (bAxisRange == 2)
		{
			if (InPointFai <= InCompareValue.z)
			{
				if (InPointFai >= InCompareValue.w)
				{
					return true;
				}
			}
		}
		else
		{
			if (IsAngleRange(InPointFai, InCompareValue.z, InCompareValue.w))
			{
				return true;
			}
		}
	}

	return false;
}

struct FCubeMapAxialRangeR
{
	float4 PositiveX;
	float4 NegativeX;

	float4 PositiveY;
	float4 NegativeY;

	float4 PositiveZ;
	float4 NegativeZ;
};

int GetSampleCubeMapIndexR(float3 InPointPosition)
{
	FCubeMapAxialRangeR CubeMapAxialRangeRight;
	CubeMapAxialRangeRight.PositiveX = float4(45.f, 135.f, 45.f, -45.f); //fai 属于 0-45  0--45
	CubeMapAxialRangeRight.NegativeX = float4(45.f, 135.f, 135.f, -135.f); //fail 属于 135-180 -135--180 //确保它已经转为CubeMapViewport下的坐标
	CubeMapAxialRangeRight.PositiveY = float4(0.f, 45.f, 360.f, -360.f); //theta 属于 0-45 //转为球面坐标
	CubeMapAxialRangeRight.NegativeY = float4(135.f, 180.f, 360.f, -360.f); //theta 属于 135-180 float3 Point = GetPointSphericalCoordinates(InPointPosition);
	CubeMapAxialRangeRight.PositiveZ = float4(45.f, 135.f, 45.f, 135.f);
	CubeMapAxialRangeRight.NegativeZ = float4(45.f, 135.f, -45.f, -135.f); //球面坐标值

	float3 Point = GetPointSphericalCoordinates(InPointPosition);

	float PointTheta = Point.y;
	float PointFai = Point.z;

	if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveX, 3, 0.f, true))
	{
		return 0;
	}
	else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeX, 0, 180.f, false))
	{
		return 1;
	}
	else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveY, 3, 0.f, false))
	{
		return 2;
	}
	else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeY, 3, 0.f, false))
	{
		return 3;
	}
	else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveZ, 1, 0.f, false))
	{
		return 4;
	}
	else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeZ, 2, 0.f, false))
	{
		return 5;
	}

	return 6;
}

float4 DebugCubeVieport(int Index)
{
	if (Index == 0)
	{
		return float4(1.f, 0.f, 0.f, 1.f);
	}
	else if (Index == 1)
	{
		return float4(1.f, 1.f, 0.f, 1.f);
	}
	else if (Index == 2)
	{
		return float4(1.f, 0.f, 1.f, 1.f);
	}
	else if (Index == 3)
	{
		return float4(0.f, 1.f, 1.f, 1.f);
	}
	else if (Index == 4)
	{
		return float4(1.f, 0.5f, 1.f, 1.f);
	}
	else if (Index == 5)
	{
		return float4(0.5f, 1.f, 1.f, 1.f);
	}

	return float4(0.f, 0.f, 0.f, 1.f);
}

float SampleBuildCDTexture(float2 InTexCoord)
{
	// 这个函数做的事情是将0-1的纹理坐标映射到[-pi, pi]范围内，这样就可以在shader中直接使用纹理坐标来计算反正切值了
	
	float pi = 3.1415926f;	// 180度
	float pi2 = 2 * pi;		// 360度
	
	float2 TexCoord = InTexCoord - 0.5f;	// 将纹理坐标转换到[-0.5, 0.5]范围内
	float2 Atan2 = atan2(TexCoord.y, TexCoord.x);	// 计算反正切值，得到[-pi, pi]范围内的值，这里为什么用atan2而不是atan呢？因为atan2可以表示所有的角度（4个相限），而atan只能表示一个相限的角度
	
	//简而言之， atan2与atan的区别体现在两个方面：
	//(1) atan2接收两个输入参数； atan只接收一个输入参数
	//(2) atan2对象限敏感， 根据两个参数判断它是属于哪个象限并给出对应的角度值，
	//值域范围[-pi, pi]；
	//atan对象限不敏感， 值域范围为[-pi / 2, pi / 2]

	
	float a = Atan2.y;	// 反正切值的y分量
	float r = 0.f;
	if (a < 0)
	{	// 如果y分量小于0, 说明处于[-pi, 0]范围内，在坐标系中也就是处于上半圆（y轴正半轴）的部分
		r = (Atan2.y + pi2) / pi2;	// 把负值的y分量映射到[0, 1]范围内

	}
	else
	{
		r = Atan2.x / pi2;	// 把正值的x分量映射到[0, 1]范围内
	}
	
	return r;
}


#endif
