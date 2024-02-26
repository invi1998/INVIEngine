#include "EngineMinimal.h"
#include "RotateArrow.h"

#include "OperationHandleSelectManage.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Construction/MacroConstruction.h"
#include "Misc/RaycastSystemLibrary.h"

extern CMeshComponent* SelectedAxisComponent;	// 被选中的轴向
extern GActorObject* SelectedActor;	// 被选中的物体

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

	// 旋转模型，使其成为正确的坐标系指向
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
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
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
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);

		if (t != 0)
		{
			// 获取物体的旋转
			XMFLOAT3 ActorRotation = SelectedActor->GetRotation();
			fvector_3d rotation = EngineMath::ToVector3d(ActorRotation);

			// 获取鼠标拖拽的旋转间距（角度）
			float angle = 0.0f;
			float offset = t - LastT2Value;
			angle = offset < 0 ? -2.25f : 2.25f;

			// TODO:这里先用simple_library库的四元数，后续考虑自己实现
			// 定义旋转
			fvector_3d deltaVector;
			XMFLOAT3 ActorDirFloat3;
			XMStoreFloat3(&ActorDirFloat3, DragDirection);
			fvector_3d dragDirection = EngineMath::ToVector3d(ActorDirFloat3);
			deltaVector = dragDirection * angle;

			// 将物体的旋转转换为欧拉角结构
			frotator rotationRotator(rotation.x, rotation.y, rotation.z);

			// 将我们的旋转角度也转为欧拉角结构
			frotator deltaRotator(deltaVector.x, deltaVector.y, deltaVector.z);

			// 将物体的欧拉角结构转换为四元数
			fquat rotationQuat;
			// 将我们的旋转角度也转为四元数
			fquat deltaQuat;

			// 此时我们得到的是物体的惯性坐标系下的旋转四元数（世界坐标系下的旋转四元数），我们需要将其转换为物体坐标系下的旋转四元数
			rotationQuat.inertia_to_object(rotationRotator);
			deltaQuat.inertia_to_object(deltaRotator);

			// 将物体的旋转四元数和计算出的旋转四元数相乘，得到新的旋转四元数，这个新的旋转四元数是物体坐标系下的
			fquat newRotationQuat = rotationQuat * deltaQuat;

			// 将新的旋转四元数转换为欧拉角，设置物体的旋转（注意先要将物体坐标系下的旋转四元数转换为世界坐标系下的旋转四元数）
			frotator newRotationRotator;
			newRotationRotator.object_to_inertia(newRotationQuat);

			XMFLOAT3 resultRotation{newRotationRotator.roll, newRotationRotator.pitch, newRotationRotator.yaw};

			SelectedActor->SetRotation(resultRotation);

			LastT2Value = t;
		}
	}
}

XMVECTOR GRotateArrow::GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation)
{
	XMFLOAT3 Dir{ 1.f, 1.f, 1.f };
	XMVECTOR OutDirection = XMLoadFloat3(&Dir);
	return OutDirection;
}
