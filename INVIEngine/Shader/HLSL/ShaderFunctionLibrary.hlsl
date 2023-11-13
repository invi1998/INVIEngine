#pragma once

/**
 * \brief 返回TBN矩阵
 * \param unitWorldNormal  单位化的世界法线
 * \param worldTangent 世界切线
 * \return 返回3x3的矩阵
 */
float3x3 GetTBNMatrix(float3 unitWorldNormal, float3 worldTangent)
{
	float3 N = unitWorldNormal;
	// T 切线等于世界切线 减去 (世界切线在世界法线上的投影 * 世界法线)
	// 其实这一步的几何意义，就是为了得到一个垂直与法线的切线T，如果我们的切线和世界法线不垂直，那么这一步可以对切线数据进行校正，得到一个垂直与世界法线的切线T
	float3 T = normalize(worldTangent - dot(worldTangent, N) * N);
	float3 B = cross(N, T);

	return float3x3(T, B, N);
}
