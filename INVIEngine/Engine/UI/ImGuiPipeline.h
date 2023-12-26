#pragma once

// #define IMGUI_IMPL_DIRECTX12_RENDERING_HELPERS
#include "imgui.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#include "Interface/DirectXDeviceInterface.h"

struct FImGuiPipeline : public IDirectXDeviceInterface_Struct
{
	FImGuiPipeline();

	void Initialize(ID3D12DescriptorHeap* heap, UINT offset);

	void Draw(float deltaTime);

	void Exit();

protected:
	void Tick(float deltaTime);
};

