#include "EngineMinimal.h"
#include "RenderLayerManage.h"

#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparantRenderLayer.h"

std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();

	CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparantRenderLayer>();
}

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayers.clear();
}

void FRenderLayerManage::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	for (auto& layer:RenderLayers)
	{
		layer->Init(geometry, directXPipelineState);
	}
}

void FRenderLayerManage::Sort()
{
	auto CmpRenderLayer = [&](const std::shared_ptr<FRenderLayer>& layerA, const std::shared_ptr<FRenderLayer>& layerB)
		{
			return layerA->GetRenderPriority() < layerB->GetRenderPriority();
		};

	std::sort(RenderLayers.begin(), RenderLayers.end(), CmpRenderLayer);
}
