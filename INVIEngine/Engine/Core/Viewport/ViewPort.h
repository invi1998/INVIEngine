#pragma once

class FViewPort
{
public:
	FViewPort();

	void ViewPortInit();

	// 定义摄像机位置，up, right, look 视口矩阵
	XMFLOAT4X4 ViewMatrix;

	// 可视范围 可视角度 投影矩阵
	XMFLOAT4X4 ProjectionMatrix;

};

