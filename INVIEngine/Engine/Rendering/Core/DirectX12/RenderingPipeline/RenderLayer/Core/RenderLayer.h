#pragma once
#include "Interface/DirectXDeviceInterface.h"
#include "Shader/Core/Shader.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/RenderingData.h"

class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
public:
	FRenderLayer();

	void RegisterRenderLayer();

	const UINT GetRenderPriority() const { return RenderPriority; }

protected:
	UINT RenderPriority;

};

