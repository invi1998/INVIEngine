#include "EngineMinimal.h"
#include "RotateArrow.h"

#include "OperationHandleSelectManage.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Construction/MacroConstruction.h"
#include "Misc/RaycastSystemLibrary.h"

extern CMeshComponent* SelectedAxisComponent;	// ��ѡ�е�����
extern GActorObject* SelectedActor;	// ��ѡ�е�����

GRotateArrow::GRotateArrow()
	: GOperationHandle()
{
	GRotateArrow::SetMesh();

	ResetColor();
}

void GRotateArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/Handle/RotateHandleX.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/Handle/RotateHandleY.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/Handle/RotateHandleZ.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, AnyAxisComponent, "Asserts/Mesh/Handle/RotateHandleZ.fbx");

	// ��תģ�ͣ�ʹ���Ϊ��ȷ������ϵָ��
	/*XAxisComponent->SetRotation({ 0.0f, 90.0f, 0.0f });
	YAxisComponent->SetRotation({ -90.0f, 0.0f, 0.0f });*/
}

void GRotateArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);

	if (!IsCurrentSelectedHandle()) return;

	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		OnMousePressed(x, y);
	}
}

void GRotateArrow::OnMouseLeftDown(int x, int y)
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

void GRotateArrow::OnMouseLeftUp(int x, int y)
{
	GOperationHandle::OnMouseLeftUp(x, y);

	if (SelectedActor && IsCurrentSelectedHandle())
	{
		SetVisible(true);
	}
}

void GRotateArrow::ExecuteInput()
{
	if (SelectedActor)
	{
		if (FInput::IsKeyPressed(Key::E))
		{
			FOperationHandleSelectManage::Get()->DisplaySelectedHandle(this);
		}
	}
}

void GRotateArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor && SelectedAxisComponent && IsCurrentSelectedHandle())
	{
		XMVECTOR ActorLocation{};		// �����λ��
		XMVECTOR DragDirection{};		// �����ק����ķ���
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
		}
	}
}

void GRotateArrow::GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation,
	XMVECTOR& OutDirection)
{
	if (SelectedAxisComponent)
	{
		XMVECTOR DragDirection{};		// �����ק����ķ���
		float t = GetMouseMoveDistance(0, 0, ActorLocation, DragDirection);
		if (t != 0)
		{
			OutDirection = DragDirection;
		}
	}
}
