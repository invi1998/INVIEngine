#pragma once

struct FSSAOConstant
{
	FSSAOConstant();

	XMFLOAT4X4 InversiveProjectionMatrix{};	// 求逆后的投影矩阵
	XMFLOAT4X4 ProjectionMatrix{};			// 摄像机的投影矩阵
	XMFLOAT4X4 TextureProjectionMatrix{};		// 纹理空间的投影矩阵

	float OcclusionRadius = 0.5f; // 遮蔽半径
	float OcclusionFadeStart = 0.2f; // 遮蔽淡出开始
	float OcclusionFadeEnd = 1.f; // 遮蔽淡出结束
	float ObscurationThreshold = 0.05f; // 遮蔽阈

	XMFLOAT4 SampleVolumeData[14]{};	// 14个随机向量
};

struct FSSAOBlurParam
{
	FSSAOBlurParam();

	float BlurWeight[12]{0};	// 3个模糊权重
	XMFLOAT2 InversionTextureSize{};	// 纹理尺寸的倒数(1.0f / width, 1.0f / height)
	float BlurRadius = 5.f; // 模糊半径
};

