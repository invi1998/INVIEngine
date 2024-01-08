#include "EngineMinimal.h"
#include "OperationHandle.h"

#include "Component/Mesh/CustomMeshComponent.h"

GOperationHandle::GOperationHandle()
{
}

void GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer)
{
	XAxisComponent->SetRenderLayerType(mesh_render_layer);
	YAxisComponent->SetRenderLayerType(mesh_render_layer);
	ZAxisComponent->SetRenderLayerType(mesh_render_layer);
}
