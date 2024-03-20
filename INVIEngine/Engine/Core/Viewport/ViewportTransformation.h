#pragma once

struct FViewportTransformation
{
	FViewportTransformation();

	XMFLOAT4 CameraPosition;
	XMFLOAT4X4 ViewProjectionMatrix;
	XMFLOAT4X4 TexViewProjectionMatrix;	// 基于纹理空间的投影矩阵
};

