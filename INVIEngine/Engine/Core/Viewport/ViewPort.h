#pragma once

class FViewPort
{
public:
	FViewPort();

	void ViewPortInit();

	void ResetViewPort(UINT width, UINT height);
	void ResetViewPortRect(UINT width, UINT height);

	// 和屏幕视口相关
	D3D12_VIEWPORT ViewPortInfo;		// 视口信息
	D3D12_RECT ViewPortRect;			// 视口裁剪矩形

	// 定义摄像机位置，up, right, look 视口矩阵
	XMFLOAT4X4 ViewMatrix;

	// 可视范围 可视角度 投影矩阵
	XMFLOAT4X4 ProjectionMatrix;

};

