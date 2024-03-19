#pragma once

class FRenderingPipeline;
class FRenderLayerManage;
class CLightManager;
class CWorld;
class CMeshManager;
class CWindowsEngine;

// 提供渲染内容的接口
class IDirectXDeviceInterface
{
	friend class IDirectXDeviceInterface_Struct;
public:
	// 开始 设置主视口的rendertarget
	void StartSetMainViewportRenderTarget();

	// 结束 设置主视口的rendertarget
	void EndSetMainViewportRenderTarget();

	// 清除主视口的交换链
	void ClearMainSwapChainCanvas();

public:
	virtual ~IDirectXDeviceInterface() = default;
	ComPtr<ID3D12Fence> GetFence();
	ComPtr<ID3D12Device> GetD3dDevice();
	CMeshManager* GetMeshManage() const;
	CLightManager* GetLightManger();
	CWorld* GetWorld();

	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	ID3D12DescriptorHeap* GetRTVHeap();
	ID3D12DescriptorHeap* GetDSVHeap();

	UINT64 GetCurrentFenceIndex();

	HWND GetMainWindowsHandle();

	[[nodiscard]] CWindowsEngine* GetEngine() const;

	class CEditorEngine* GetEditorEngine();

	ID3D12Resource* GetDepthBufferResource();

protected:
	FRenderingPipeline* GetRenderPipeline() const;
	FRenderLayerManage* GetRenderLayerManager() const;

};

struct IDirectXDeviceInterface_Struct
{
public:
	// 开始 设置主视口的rendertarget
	void StartSetMainViewportRenderTarget();

	// 结束 设置主视口的rendertarget
	void EndSetMainViewportRenderTarget();

	// 清除主视口的交换链
	void ClearMainSwapChainCanvas();
public:
	ComPtr<ID3D12Fence> GetFence();
	ComPtr<ID3D12Device> GetD3dDevice();
	CMeshManager* GetMeshManage();
	CLightManager* GetLightManger();
	CWorld* GetWorld();

	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	UINT64 GetCurrentFenceIndex();

	HWND GetMainWindowsHandle();

	[[nodiscard]] CWindowsEngine* GetEngine();

	class CEditorEngine* GetEditorEngine();

	ID3D12Resource* GetDepthBufferResource();

protected:
	FRenderingPipeline* GetRenderPipeline() const;
	FRenderLayerManage* GetRenderLayerManager() const;

private:
	IDirectXDeviceInterface Interface{};

};


