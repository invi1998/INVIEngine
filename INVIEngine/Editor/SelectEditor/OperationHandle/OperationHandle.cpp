#include "EngineMinimal.h"
#include "OperationHandle.h"

#include "Component/Mesh/CustomMeshComponent.h"
#include "Material/Core/Material.h"

GOperationHandle::GOperationHandle()
{
}

void GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer)
{
	XAxisComponent->SetRenderLayerType(mesh_render_layer);
	YAxisComponent->SetRenderLayerType(mesh_render_layer);
	ZAxisComponent->SetRenderLayerType(mesh_render_layer);
}

void GOperationHandle::ResetColor()
{
	ResetColor(XAxisComponent, {1.f, 0.f, 0.f, 1.f});
	ResetColor(YAxisComponent, {0.f, 1.f, 0.f, 1.f});
	ResetColor(ZAxisComponent, {0.f, 0.f, 1.f, 1.f});
}

void GOperationHandle::ResetColor(CCustomMeshComponent* axis_component, const XMFLOAT4& color)
{
	if (axis_component)
	{
		if (CMaterial* material = (*axis_component->GetMaterial())[0])
		{
			material->SetBaseColor(color);
		}
	}
}
