#include "DirectXDeviceInterface.h"

#include "Platform/Windows/WindowsEngine.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->Fence;
		}
	}

	return nullptr;
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice()
{
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetD3dGraphicsCommandList()
{
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
{
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
{
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
{
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
{
}

CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
}




/**
 *	---------------------------------------------------------------------------------------------------------
 */


ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence()
{
	return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice()
{
	return Interface.GetD3dDevice();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetD3dGraphicsCommandList()
{
	return Interface.GetD3dGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator()
{
	return Interface.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue()
{
	return Interface.GetCommandQueue();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex()
{
	return Interface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle()
{
	return Interface.GetMainWindowsHandle();
}

CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
	return Interface.GetEngine();
}
