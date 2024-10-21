#include "EngineMinimal.h"

#include "MeshComponent.h"

#include "Material/Core/Material.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"

CMeshComponent::CMeshComponent()
	: MeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_TRANSPARENT)
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = this;

	Materials.push_back(CreateObject<CMaterial>(params, new CMaterial()));
}

CMeshComponent::~CMeshComponent()
{
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();
}

void CMeshComponent::BeginInit()
{
	CTransformationComponent::BeginInit();
}

void CMeshComponent::Tick(float DeltaTime)
{
	CTransformationComponent::Tick(DeltaTime);
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::Init()
{
}

void CMeshComponent::SetRenderLayerType(EMeshRenderLayerType type)
{
	MeshRenderLayerType = type;
}

bool CMeshComponent::IsDynamicReflection() const
{
	if (Materials.size() > 0)
	{
		return Materials[0]->IsDynamicReflection() && MeshRenderLayerType == EMeshRenderLayerType::RENDER_LAYER_OPAQUE_REFLECT;
	}

	return false;
}

bool CMeshComponent::IsCastShadow() const
{
	return bCastShadow;
}

void CMeshComponent::GetBoundingBox(BoundingBox& box)
{
	FGeometry::FindRenderingData([&](std::shared_ptr<FRenderingData>& renderingData)->EFindValueType const
	{
		if (renderingData->Mesh == this)
		{
			box = renderingData->Bounds;
			return EFindValueType::FVT_COMPLETE;
		}

		return EFindValueType::FVT_IN_PROGRAM;
	});
}

BoundingBox CMeshComponent::GetBoundingBox()
{
	BoundingBox box{};
	GetBoundingBox(box);
	return box;
}
