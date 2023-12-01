#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FDynamicMap : public IDirectXDeviceInterface
{
public:
		FDynamicMap();
		~FDynamicMap() override = default;

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.cpp
