#include "EngineMinimal.h"

#include "DirectXDeviceInterface.h"

#include "Platform/Windows/WindowsEngine.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

void IDirectXDeviceInterface::StartSetMainViewportRenderTarget()
{
	GetEngine()->GetRenderingEngine()->StartSetMainViewportRenderTarget();
}

void IDirectXDeviceInterface::EndSetMainViewportRenderTarget()
{
	GetEngine()->GetRenderingEngine()->EndSetMainViewportRenderTarget();
}

void IDirectXDeviceInterface::ClearMainSwapChainCanvas()
{
	GetEngine()->GetRenderingEngine()->ClearMainSwapChainCanvas();
}

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

CMeshManager* IDirectXDeviceInterface::GetMeshManage() const
{
	return GetEngine()->GetMeshManage();
}

CLightManager* IDirectXDeviceInterface::GetLightManger()
{
	if (CWindowsEngine* InEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GetLightManager();;
		}
	}

	return nullptr;
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

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetRTVHeap()
{
	if (CWindowsEngine* inEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->RTVHeap.Get();
		}
	}
	return nullptr;
}

ID3D12DescriptorHeap* IDirectXDeviceInterface::GetDSVHeap()
{
	if (CWindowsEngine* inEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->DSVHeap.Get();
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

CWindowsEngine* IDirectXDeviceInterface::GetEngine() const
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}

CEditorEngine* IDirectXDeviceInterface::GetEditorEngine()
{
	if (CWindowsEngine* inEngine = GetEngine())
	{
		return inEngine->EditorEngine;
	}

	return nullptr;
}

FRenderingPipeline* IDirectXDeviceInterface::GetRenderPipeline() const
{
	if (CMeshManager* inMeshManager = GetMeshManage())
	{
		return inMeshManager->GetRenderingPipeline();
	}
	return nullptr;
}

FRenderLayerManage* IDirectXDeviceInterface::GetRenderLayerManager() const
{
	if (FRenderingPipeline* inRenderingPipeline = GetRenderPipeline())
	{
		return inRenderingPipeline->GetRenderLayerManage();
	}
	return nullptr;
}

void IDirectXDeviceInterface_Struct::StartSetMainViewportRenderTarget()
{
	if (CWindowsEngine* inEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->StartSetMainViewportRenderTarget();
		}
	}
}

void IDirectXDeviceInterface_Struct::EndSetMainViewportRenderTarget()
{
	if (CWindowsEngine* inEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->EndSetMainViewportRenderTarget();
		}
	}
}

void IDirectXDeviceInterface_Struct::ClearMainSwapChainCanvas()
{
	if (CWindowsEngine* inEngine = dynamic_cast<CWindowsEngine*>(Engine))
	{
		if (inEngine->GetRenderingEngine())
		{
			return inEngine->GetRenderingEngine()->ClearMainSwapChainCanvas();
		}
	}
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

CMeshManager* IDirectXDeviceInterface_Struct::GetMeshManage()
{
	return Interface.GetMeshManage();
}

CLightManager* IDirectXDeviceInterface_Struct::GetLightManger()
{
	return Interface.GetLightManger();
}

CWorld* IDirectXDeviceInterface_Struct::GetWorld()
{
	return Interface.GetWorld();
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

CEditorEngine* IDirectXDeviceInterface_Struct::GetEditorEngine()
{
	return Interface.GetEditorEngine();
}

FRenderingPipeline* IDirectXDeviceInterface_Struct::GetRenderPipeline() const
{
	return Interface.GetRenderPipeline();
}

FRenderLayerManage* IDirectXDeviceInterface_Struct::GetRenderLayerManager() const
{
	return Interface.GetRenderLayerManager();
}
