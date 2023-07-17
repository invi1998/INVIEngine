#pragma once

#include "Core/Engine.h"
#include "Platform/Windows/WindowsEngine.h"

class IRenderingInterface // : public IGuidInterface
{
	// friend class CDirectXRenderingEngine;
	friend class IDirectXDeviceInterface;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();	// override;

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

protected:
	// 构造默认缓冲区
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize);

	ComPtr<ID3D12Fence> GetFence();
	ComPtr<ID3D12Device> GetD3dDevice();

	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	UINT64 GetCurrentFenceIndex();

	HWND GetMainWindowsHandle();

	[[nodiscard]] CWindowsEngine* GetEngine();

//private:
//	static std::vector<IRenderingInterface*> RenderingInterface;
};

