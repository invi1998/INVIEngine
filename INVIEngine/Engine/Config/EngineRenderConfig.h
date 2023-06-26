#pragma once

// ������Ⱦ��ص�����

struct FEngineRenderConfig
{
	FEngineRenderConfig();

	int ScreenWidth;			// ��Ļ���
	int ScreenHeight;			// ��Ļ�߶�
	int RefreshRate;			// ֡��
	int SwapChainCount;			// ����������

	static FEngineRenderConfig* GetRenderConfig();
	static void Destroy();

private:
	static FEngineRenderConfig* RenderConfig;

};
