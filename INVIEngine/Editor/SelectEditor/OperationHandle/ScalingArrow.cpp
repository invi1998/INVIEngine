#include "EngineMinimal.h"
#include "ScalingArrow.h"

#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"

extern CMeshComponent* SelectedAxisComponent;	// 被选中的轴向
extern GActorObject* SelectedActor;	// 被选中的物体

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

	YAxisComponent->SetRotation({ -90.f, 0.f, 0.f });
	XAxisComponent->SetRotation({ 0.f, 90.f, 0.f });
}

void GScalingArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);

	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		OnMousePressed(x, y);
	}
}

void GScalingArrow::OnMouseLeftDown(int x, int y)
{
	GOperationHandle::OnMouseLeftDown(x, y);

	if (SelectedAxisComponent)
	{
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
			LastT2Value = t;
		}
	}
}

void GScalingArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor and SelectedAxisComponent)
	{
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
			XMVECTOR CurrentScale = XMLoadFloat3(&SelectedActor->GetScale());
			
			float OffsetScale = t - LastT2Value;
			float value = OffsetScale < 0.f ? -0.25f : 0.25f;
			CurrentScale += (DragDirection * value);
			SelectedActor->SetScale(CurrentScale);
		}
	}
}
