#include "EngineMinimal.h"
#include "ScalingArrow.h"

#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"

GScalingArrow::GScalingArrow() : GOperationHandle()
{
	GScalingArrow::SetMesh();

	ResetColor();
}

void GScalingArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/Handle/ScalingArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/Handle/ScalingArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/Handle/ScalingArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, AnyAxisComponent, "Asserts/Mesh/Handle/AnyAxis_Type_1.fbx");
}

void GScalingArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);
}

void GScalingArrow::OnMouseLeftDown(int x, int y)
{
	GOperationHandle::OnMouseLeftDown(x, y);
}

void GScalingArrow::OnMousePressed(int x, int y)
{
}
