#pragma once

#include "Core/Viewport/ViewportInfo.h"
#include "Mesh/Core/MeshManager.h"
#include "Rendering/Engine/Core/RenderingEngine.h"

class CLightManager;

class CDirectXRenderingEngine : public CRenderingEngine
{
	friend class CWindowsEngine;

	friend class IDirectXDeviceInterface;

public:
	CDirectXRenderingEngine();
	~CDirectXRenderingEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init(FWinMainCommandParameters InParameters) override;
	virtual int PostInit() override;

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info);

	virtual void Tick(float DeltaTime) override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

	// 开始 设置主视口的rendertarget
	void StartSetMainViewportRenderTarget();

	// 结束 设置主视口的rendertarget
	void EndSetMainViewportRenderTarget();

	// 清除主视口的交换链
	void ClearMainSwapChainCanvas();

public:

	ID3D12Resource* GetCurrentSwapBuffer() const;	// 获取当前交换链buff缓冲区
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;		// 获取当前交换链buffer的视口句柄
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;		// 获取当前深度模板View

	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }			// 获取后台缓冲区格式
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }		// 获取深度模板缓冲区格式
	UINT GetDXGISampleCount() const;												// 获取采样数量
	UINT GetDXGISampleQuality() const;												// 获取采样质量
	CMeshManager* GetMeshManage();
	CLightManager* GetLightManager();

protected:
	void WaitGPUCommandQueueComplete();		// 等待GPU执行命令队列 同步GPU和CPU
	// 初始化Direct3D
	bool InitDirect3D();
	void PostInitDirect3D();


protected:
	UINT64 CurrentFenceIndex;		// 当前围栏索引
	int CurrentSwapBufferIndex;		// 当前交换链buffer缓冲区的索引（我们在做缓冲区交换的时候，就会去修改这个值）

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

	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer{};		// 交换链Buffer（包括两个缓冲区，一个前台缓冲区，一个后台缓冲区）
	ComPtr<ID3D12Resource> DepthStencilBuffer;					// 深度模板缓冲区

protected:

	// 当前采样的质量级别(4采样数的质量级别）
	UINT M4XNumQualityLevels;

	// 4重采样是否开启
	bool bMSAA4XEnabled;

	// 纹理buffer格式
	DXGI_FORMAT BackBufferFormat;	// 后台缓冲区格式
	DXGI_FORMAT DepthStencilFormat; // 深度模板缓冲区格式

	UINT RTVDescriptorSize;			// RTV描述符大小

protected:
	CLightManager* LightManager;
	CMeshManager* MeshManage;
	CWorld* World;

};

