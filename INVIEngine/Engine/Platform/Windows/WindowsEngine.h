#pragma once

#include "Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	FWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init(FWinMainCommandParameters InParameters) override;
	virtual int PostInit() override;

	virtual void Tick() override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

protected:
	// 驱动对象
	ComPtr<IDXGIFactory4> DXGiFactory;	// 创建DirectX图形基础结构（DXGi）对象
	ComPtr<ID3D12Device> D3dDevice;		// 创建命令分配器，命令列表，命令队列，Fence，资源，管道状态对象，堆，根签名，采样器和许多资源视图
	ComPtr<ID3D12Fence> Fence;			// 一个用于同步CPU和一个或者多个GPU的对象

	// 命令对象
	ComPtr<ID3D12CommandQueue> CommandQueue;				// 命令队列
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// 命令分配器（做存储命令和分配内存）
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// 图形命令列表

	// 交换链对象
	ComPtr<IDXGISwapChain> SwapChain;		// 交换链

	// 描述符对象和堆
	ComPtr<ID3D12DescriptorHeap> RTVHeap;		// 渲染目标视图
	ComPtr<ID3D12DescriptorHeap> DSVHeap;		// 深度模板视图

protected:
	// 主窗口句柄
	HWND MainWindowHandle;

	// 当前采样的质量级别(4采样数的质量级别）
	UINT M4XNumQualityLevels;

	// 4重采样是否开启
	bool bMSAA4XEnabled;

	// 纹理buffer格式
	DXGI_FORMAT BufferFormat;


private:
	// 初始化窗口
	bool InitWindows(FWinMainCommandParameters InParameters);

	// 初始化Direct3D
	bool InitDirect3D();

};
