#include "EngineMinimal.h"
#include "RenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"

FRenderLayer::FRenderLayer(): RenderPriority(0)
{
}

void FRenderLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderLayers.push_back(this->shared_from_this());
}
