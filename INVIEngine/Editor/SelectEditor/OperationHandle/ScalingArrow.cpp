#include "EngineMinimal.h"
#include "ScalingArrow.h"

#include "OperationHandleSelectManage.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"
#include "Misc/RaycastSystemLibrary.h"

extern CMeshComponent* SelectedAxisComponent;	// ��ѡ�е�����
extern GActorObject* SelectedActor;	// ��ѡ�е�����

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

	if (!IsCurrentSelectedHandle()) return;

	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		OnMousePressed(x, y);
	}
}

void GScalingArrow::OnMouseLeftDown(int x, int y)
{
	GOperationHandle::OnMouseLeftDown(x, y);

	if (SelectedActor && IsCurrentSelectedHandle())
	{
		XMFLOAT2 mousePos(x, y);

		EngineType::FHitResult HitResult{};
		FRayCastSystemLibrary::CheckObjectIsSelected(GetWorld(), mousePos, this, HitResult);

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
		XMVECTOR ActorLocation{};		// �����λ��
		XMVECTOR DragDirection{};		// �����ק����ķ���
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
			LastT2Value = t;
		}
	}
}

void GScalingArrow::OnMouseLeftUp(int x, int y)
{
	GOperationHandle::OnMouseLeftUp(x, y);

	if (SelectedActor && IsCurrentSelectedHandle())
	{
		SetVisible(true);
	}
}

void GScalingArrow::ExecuteInput()
{
	if (SelectedActor)
	{
		if (FInput::IsKeyPressed(Key::R))
		{
			FOperationHandleSelectManage::Get()->DisplaySelectedHandle(this);
		}
	}
}

void GScalingArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor && SelectedAxisComponent && IsCurrentSelectedHandle())
	{
		XMVECTOR ActorLocation{};		// �����λ��
		XMVECTOR DragDirection{};		// �����ק����ķ���
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
