#include "EngineMinimal.h"
#include "RenderLayerManage.h"

#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/BackgroundRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"

std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();

	CreateRenderLayer<FBackgroundRenderLayer>();
	// CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
}

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayers.clear();
}

void FRenderLayerManage::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	for (const auto& layer:RenderLayers)
	{
		layer->Init(geometry, directXPipelineState);
	}
}

void FRenderLayerManage::PreDraw(float deltaTime)
{
	for (const auto& layer : RenderLayers)
	{
		layer->PreDraw(deltaTime);
	}
}

void FRenderLayerManage::Draw(float deltaTime)
{
	for (const auto& layer : RenderLayers)
	{
		layer->Draw(deltaTime);
	}
}

void FRenderLayerManage::PostDraw(float deltaTime)
{
	for (const auto& layer : RenderLayers)
	{
		layer->PostDraw(deltaTime);
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

void FRenderLayerManage::BuildShader()
{
	for (const auto& layer : RenderLayers)
	{
		layer->BuildShader();
	}
}

void FRenderLayerManage::BuildPSO()
{
	for (const auto& layer : RenderLayers)
	{
		layer->BuildPSO();
	}
}

std::shared_ptr<FRenderLayer> FRenderLayerManage::FindByRenderLayer(int layer)
{
	for (auto& layerData : RenderLayers)
	{
		if (layerData->GetRenderLayerType() == layer)
		{
			return layerData;
		}
	}

	return nullptr;
}

void FRenderLayerManage::UpdateCaculations(float deltaTime, const FViewportInfo& viewportInfo)
{
	for (const auto& layer : RenderLayers)
	{
		layer->UpdateCaculations(deltaTime, viewportInfo);
	}
}
