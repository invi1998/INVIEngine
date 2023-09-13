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
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->D3dDevice;
		}
	}

	return nullptr;
}

CMeshManager* IDirectXDeviceInterface::GetMeshManage()
{
	return GetEngine()->GetMeshManage();
}

CWorld* IDirectXDeviceInterface::GetWorld()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->GetWorld();
	}

	return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetD3dGraphicsCommandList()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;
		}
	}

	return nullptr;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandAllocator;
		}
	}

	return nullptr;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandQueue;
		}
	}

	return nullptr;
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CurrentFenceIndex;
		}
	}

	return 0;
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->MainWindowsHandle;
		}
	}

	return HWND();
}

CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(Engine);
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
