#include "EngineMinimal.h"
#include "RenderLayerManage.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/World.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/BackgroundRenderLayer.h"
#include "RenderLayer/OpaqueReflectorRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/OpaqueShadowRenderLayer.h"
#include "RenderLayer/OperationHandleRenderLayer.h"
#include "RenderLayer/OperationHandleRotPlaneRenderLayer.h"
#include "RenderLayer/SelectRenderLayer.h"
#include "RenderLayer/SSAOBilateralRenderLayer.h"
#include "RenderLayer/SSAORenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"

std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();

	CreateRenderLayer<FSelectRenderLayer>();
	CreateRenderLayer<FBackgroundRenderLayer>();
	// CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
	CreateRenderLayer<FOpaqueReflectorRenderLayer>();
	CreateRenderLayer<FOpaqueShadowRenderLayer>();
	CreateRenderLayer<FOperationHandleRenderLayer>();
	CreateRenderLayer<FOperationHandleRotPlaneRenderLayer>();
	CreateRenderLayer<FSSAORenderLayer>();
	CreateRenderLayer<FSSAOBilateralRenderLayer>();
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

void FRenderLayerManage::Draw(int inLayer, float deltaTime)
{
	if (const auto layer = FindByRenderLayer(inLayer))
	{
		layer->Draw(deltaTime);
	}
}

void FRenderLayerManage::FindObjectDraw(float DeltaTime, int layer, const CMeshComponent* key)
{
	if (const auto innerLayer = FindByRenderLayer(layer))
	{
		innerLayer->FindObjectDraw(DeltaTime, key);
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

void FRenderLayerManage::BuildPSO()
{
	for (const auto& layer : RenderLayers)
	{
		layer->BuildPSO();
	}
}

void FRenderLayerManage::HighlightObject(GActorObject* object)
{
	FGeometry::FindRenderingData(
		[&](std::shared_ptr<FRenderingData>& renderDate)->EFindValueType
		{
			if (GActorObject* actorObject = dynamic_cast<GActorObject*>(renderDate->Mesh->GetOwner()))
			{
				if (actorObject == object)
				{
					HighlightObject(renderDate);
					return EFindValueType::FVT_COMPLETE;
				}
			}
			return EFindValueType::FVT_IN_PROGRAM;
		}
	);
}

extern int ActorSelected;	// 被选中的Actor的ID
void FRenderLayerManage::HighlightObject(std::weak_ptr<FRenderingData> weakRenderDate)
{
	// 清除旧的高亮
	Clear(EMeshRenderLayerType::RENDER_LAYER_SELECT);

	// 设置新的高亮
	Add(weakRenderDate, EMeshRenderLayerType::RENDER_LAYER_SELECT);

	// 记录被选中的Actor的ID
	if (const auto renderDate = weakRenderDate.lock())
	{
		if (GActorObject* actorObject = dynamic_cast<GActorObject*>(renderDate->Mesh->GetOwner()))
		{
			const std::vector<GActorObject*>& actors = GetWorld()->GetActors();
			for (int i = 0; i < actors.size(); ++i)
			{
				if (actors[i] == actorObject)
				{
					ActorSelected = i;
					break;
				}
			}
		}
	}
}

void FRenderLayerManage::HighlightObject(CMeshComponent* commponent)
{
	if (CMeshComponent* meshComponent = dynamic_cast<CMeshComponent*>(commponent))
	{
		FGeometry::FindRenderingData(
		[&](std::shared_ptr<FRenderingData>& renderDate)->EFindValueType
		{
			if (renderDate->Mesh == meshComponent)
			{
				HighlightObject(renderDate);
				return EFindValueType::FVT_COMPLETE;
			}
			return EFindValueType::FVT_IN_PROGRAM;
		});		
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

void FRenderLayerManage::ResetPSO(int layer)
{
	if (const auto innerLayer = FindByRenderLayer(layer))
	{
		innerLayer->ResetPSO();
	}
}

void FRenderLayerManage::ResetPSO(int layer, EPipelineState ps)
{
	if (const auto innerLayer = FindByRenderLayer(layer))
	{
		innerLayer->ResetPSO(ps);
	}
}

void FRenderLayerManage::DrawMesh(float DeltaTime, int layer, ERenderCondition rc)
{
	if (const auto innerLayer = FindByRenderLayer(layer))
	{
		innerLayer->DrawMesh(DeltaTime, rc);
	}
}

void FRenderLayerManage::Add(std::weak_ptr<FRenderingData>& weakRenderDate, int inLayer)
{
	if (const auto renderData = weakRenderDate.lock())
	{
		if (const auto layer = FindByRenderLayer(inLayer))
		{
			layer->Add(weakRenderDate);
		}
	}
}

void FRenderLayerManage::Remove(std::weak_ptr<FRenderingData>& weakRenderDate, int inLayer)
{
	if (const auto renderData = weakRenderDate.lock())
	{
		if (const auto layer = FindByRenderLayer(inLayer))
		{
			layer->Remove(weakRenderDate);
		}
	}
}

void FRenderLayerManage::Remove(const size_t hashKey, int inLayer)
{
	if (const auto layer = FindByRenderLayer(inLayer))
	{
		layer->Remove(hashKey);
	}
}

void FRenderLayerManage::Clear(int layer)
{
	if (const auto innerLayer = FindByRenderLayer(layer))
	{
		innerLayer->Clear();
	}
}

