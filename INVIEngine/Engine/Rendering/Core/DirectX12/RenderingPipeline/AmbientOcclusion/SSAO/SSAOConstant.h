#pragma once

struct FSSAOConstant
{
	FSSAOConstant();

	XMFLOAT4X4 InversiveProjectionMatrix{};	// 求逆后的投影矩阵
	XMFLOAT4X4 ProjectionMatrix{};			// 摄像机的投影矩阵
	XMFLOAT4X4 TextureProjectionMatrix{};		// 纹理空间的投影矩阵

	XMFLOAT4 OffsetVectors[14]{};	// 14个随机向量
};

