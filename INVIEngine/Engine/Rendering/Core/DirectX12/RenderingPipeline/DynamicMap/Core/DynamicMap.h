#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FRenderLayerManage;
struct FDirectXPipelineState;
struct FGeometryMap;
struct FViewportInfo;

class FDynamicMap : public IDirectXDeviceInterface
{
public:
	FDynamicMap();
	~FDynamicMap() override = default;
	

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.cpp
