#pragma once

#include "EngineMinimal.h"
#include "Core/Engine.h"


class CWindowsEngine;

// �ṩ��Ⱦ���ݵĽӿ�
class IDirectXDeviceInterface
{
public:
	ComPtr<ID3D12Fence> GetFence();
	ComPtr<ID3D12Device> GetD3dDevice();

	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	UINT64 GetCurrentFenceIndex();

	HWND GetMainWindowsHandle();

	[[nodiscard]] CWindowsEngine* GetEngine();

};

struct IDirectXDeviceInterface_Struct
{
public:
	ComPtr<ID3D12Fence> GetFence();
	ComPtr<ID3D12Device> GetD3dDevice();

	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	UINT64 GetCurrentFenceIndex();

	HWND GetMainWindowsHandle();

	[[nodiscard]] CWindowsEngine* GetEngine();

private:
	IDirectXDeviceInterface Interface;

};

