#include "EngineMinimal.h"
#include "MoveArrow.h"

#include "OperationHandleSelectManage.h"
#include "Collision/CollisionSceneQuery.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"
#include "Misc/RaycastSystemLibrary.h"

extern CMeshComponent* SelectedAxisComponent;	// 被选中的轴向
extern GActorObject* SelectedActor;	// 被选中的物体

GMoveArrow::GMoveArrow() : GOperationHandle()
{
	// 设置模型
	GMoveArrow::SetMesh();

	ResetColor();

}

void GMoveArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/Handle/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/Handle/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/Handle/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, AnyAxisComponent, "Asserts/Mesh/Handle/AnyAxis_Type_1.fbx");

	// 旋转模型，使其成为正确的坐标系指向
	XAxisComponent->SetRotation(XMFLOAT3{ 0.0f, 90.0f, 0.0f });
	YAxisComponent->SetRotation(XMFLOAT3{ -90.0f, 0.0f, 0.0f });

	// ResetColor();

}

void GMoveArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);

	if (!IsCurrentSelectedHandle()) return;

	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		OnMousePressed(x, y);
	}
}

void GMoveArrow::OnMouseLeftDown(int x, int y)
{
	GOperationHandle::OnMouseLeftDown(x, y);

	if (SelectedActor && IsCurrentSelectedHandle())
	{
		XMFLOAT2 mousePos(x, y);

		EngineType::FHitResult HitResult{};
		FRayCastSystemLibrary::CheckObjectIsSelected(GetWorld(), mousePos, this, HitResult, GetIgnoreComponents());

		if (HitResult.bHit)
		{
			if (HitResult.HitComponent == XAxisComponent)
			{
				SelectedAxisComponent = XAxisComponent;
				SetVisible(true, XAxisComponent);
				SetVisible(false, YAxisComponent);
				SetVisible(false, ZAxisComponent);
			}
			else if (HitResult.HitComponent == YAxisComponent)
			{
				SelectedAxisComponent = YAxisComponent;
				SetVisible(true, YAxisComponent);
				SetVisible(false, XAxisComponent);
				SetVisible(false, ZAxisComponent);
			}
			else if (HitResult.HitComponent == ZAxisComponent)
			{
				SelectedAxisComponent = ZAxisComponent;
				SetVisible(true, ZAxisComponent);
				SetVisible(false, YAxisComponent);
				SetVisible(false, XAxisComponent);
			}
		}
		else
		{
			SelectedAxisComponent = nullptr;
			SetVisible(true);
		}
	}
	else
	{
		return;
	}

	if (SelectedAxisComponent)
	{
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
			// 计算出经过时间t后，射线的方向向量和鼠标拖拽的轴的方向向量的相交点（模型被拖拽的目标点）
			XMVECTOR DragPoint = ActorLocation + DragDirection * t;

			RelativePosition = DragPoint - ActorLocation;
		}
	}
}

void GMoveArrow::OnMouseLeftUp(int x, int y)
{
	GOperationHandle::OnMouseLeftUp(x, y);

	if (SelectedActor && IsCurrentSelectedHandle())
	{
		SetVisible();
	}
}

void GMoveArrow::ExecuteInput()
{
	if (SelectedActor)
	{
		if (FInput::IsKeyPressed(Key::W))
		{
			FOperationHandleSelectManage::Get()->DisplaySelectedHandle(this);

			SetVisible();
		}
	}
}

void GMoveArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor && SelectedAxisComponent && IsCurrentSelectedHandle())
	{
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
			// 计算出经过时间t后，射线的方向向量和鼠标拖拽的轴的方向向量的相交点（模型被拖拽的目标点）
			XMVECTOR DragPoint = ActorLocation + DragDirection * t - RelativePosition;

			// 设置物体的位置
			SelectedActor->SetPosition(DragPoint);

			// 设置操作句柄的位置
			SetPosition(DragPoint);
		}
	}
}

XMVECTOR GMoveArrow::GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation)
{
	return XMVector3Normalize(WorldDirection);
}


