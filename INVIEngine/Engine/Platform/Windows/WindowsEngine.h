#pragma once

#include "Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	FWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init() override;
	virtual int PostInit() override;

	virtual void Tick() override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

protected:
	// ��������
	ComPtr<IDXGIFactory4> DXGiFactory;	// ����DirectXͼ�λ����ṹ��DXGi������
	ComPtr<ID3D12Device> D3dDevice;		// ��������������������б�������У�Fence����Դ���ܵ�״̬���󣬶ѣ���ǩ�����������������Դ��ͼ
	ComPtr<ID3D12Fence> Fence;			// һ������ͬ��CPU��һ�����߶��GPU�Ķ���

	// �������
	ComPtr<ID3D12CommandQueue> CommandQueue;				// �������
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// ��������������洢����ͷ����ڴ棩
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// ͼ�������б�

	// ����������
	ComPtr<IDXGISwapChain> SwapChain;		// ������

	// �����ھ��
	HWND MainWindowHandle;

	// ��ǰ��������������(4����������������
	UINT M4XNumQualityLevels;

	// 4�ز����Ƿ���
	bool bMSAA4XEnabled;

private:
	// ��ʼ������
	bool InitWindows(FWinMainCommandParameters InParameters);

	// ��ʼ��Direct3D
	bool InitDirect3D();

};
