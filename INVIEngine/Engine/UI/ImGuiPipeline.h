#pragma once

#define IMGUI_IMPL_DIRECTX12_RENDERING_HELPERS
#include "imgui.h"
#include "Interface/DirectXDeviceInterface.h"
// #include "imgui_impl_win32.h"

struct FImGuiPipeline : public IDirectXDeviceInterface_Struct
{
	FImGuiPipeline();

	void Initialize(ID3D12DescriptorHeap* heap, UINT offset);

	void Draw(float deltaTime);

	void Exit();
};

