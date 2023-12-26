#pragma once

#define IMGUI_IMPL_DIRECTX12_RENDERING_HELPERS
#include "imgui.h"
#include "Interface/DirectXDeviceInterface.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

struct FImGuiPipeline : public IDirectXDeviceInterface_Struct
{
	FImGuiPipeline();

	void Initialize(ID3D12DescriptorHeap* heap, UINT offset);

	void Draw(float deltaTime);

	void Exit();
};

