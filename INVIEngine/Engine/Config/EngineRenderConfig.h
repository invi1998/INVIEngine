#pragma once

// 引擎渲染相关的配置

struct FEngineRenderConfig
{
	FEngineRenderConfig();

	int ScreenWidth;			// 屏幕宽度
	int ScreenHeight;			// 屏幕高度
	int RefreshRate;			// 帧率
	int SwapChainCount;			// 交换链数量

	static FEngineRenderConfig* GetRenderConfig();
	static void Destroy();

private:
	static FEngineRenderConfig* RenderConfig;

};
