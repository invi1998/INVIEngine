#include "EngineMinimal.h"
#include "RotateArrow.h"

#include "OperationHandleSelectManage.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/QuaternionCamera.h"
#include "Core/World.h"
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
			frotator rotationRotator(rotation.y, rotation.z, rotation.x);

			// 将我们的旋转角度也转为欧拉角结构
			frotator deltaRotator(deltaVector.y, deltaVector.z, deltaVector.x);

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

void GRotateArrow::SetScale(const XMFLOAT3& InNewScale)
{
	GOperationHandle::SetScale(InNewScale);

	if (XAxisComponent && YAxisComponent && ZAxisComponent)
	{
		AnyAxisComponent->SetScale(XMFLOAT3{InNewScale.x*1.5f, InNewScale.y*1.5f, InNewScale.z*1.5f});

		XAxisComponent->SetScale(InNewScale);
		YAxisComponent->SetScale(InNewScale);
		ZAxisComponent->SetScale(InNewScale);
	}
}

void GRotateArrow::Tick(float DeltaTime)
{
	GOperationHandle::Tick(DeltaTime);

	if (IsCurrentSelectedHandle())
	{
		// 我们需要将摄像机的位置转换为操作手柄的坐标系下的位置
		XMFLOAT4X4 ArrowMatrix{};
		EngineMath::BuildMatrix(ArrowMatrix, GetPosition(), GetScale(), GetRightVector(), GetUpVector(), GetForwardVector());

		XMMATRIX ArrowMatrixXM = XMLoadFloat4x4(&ArrowMatrix);		// 操作手柄的世界矩阵
		XMVECTOR ArrowMatrixDetemine = XMMatrixDeterminant(ArrowMatrixXM);	// 操作手柄的世界矩阵的行列式s
		XMMATRIX ArrowMatrixInverse = XMMatrixInverse(&ArrowMatrixDetemine, ArrowMatrixXM);	// 操作手柄的世界矩阵的逆矩阵

		XMVECTOR CameraLocation = XMLoadFloat3(&GetWorld()->GetQuaternionCamera()->GetPosition());	// 摄像机的位置

		XMVECTOR ArrowMatrixInverseLocation = XMVector3TransformCoord(CameraLocation, ArrowMatrixInverse);	// 摄像机的位置转换为操作手柄的坐标系下的位置

		XMFLOAT3 ArrowMatrixInverseLocationFloat3;
		XMStoreFloat3(&ArrowMatrixInverseLocationFloat3, ArrowMatrixInverseLocation);

		int type = EngineMath::GetSample8CubeIndex(EngineMath::ToVector3d(ArrowMatrixInverseLocationFloat3));

		switch (type)
		{
		case 0:
			YAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));//绿
			XAxisComponent->SetRotation(frotator(-90.f, 0, 0.f));//红色
			AnyAxisComponent->SetRotation(frotator());
			ZAxisComponent->SetRotation(frotator());
			break;
		case 1:
			YAxisComponent->SetRotation(frotator(0.f, -180.f, 0.f));//绿
			XAxisComponent->SetRotation(frotator(-90.f, 0, 0.f));//红色
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			break;
		case 2:
			YAxisComponent->SetRotation(frotator(0.f, -90.f, 0.f));
			XAxisComponent->SetRotation(frotator());
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -90.f));
			break;
		case 3:
			YAxisComponent->SetRotation(frotator());//绿
			XAxisComponent->SetRotation(frotator());//红色
			AnyAxisComponent->SetRotation(frotator());//青色
			ZAxisComponent->SetRotation(frotator());//蓝色
			break;
		case 4:
			YAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));//绿
			XAxisComponent->SetRotation(frotator(-180.f, 0, 0.f));//红色
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//青色
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//蓝色
			break;
		case 5:
			YAxisComponent->SetRotation(frotator(0.f, 180.f, 0.f));//绿
			XAxisComponent->SetRotation(frotator(-180.f, 0, 0.f));//红色
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//青色
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//蓝色
			break;
		case 6:
			YAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//绿
			XAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));//红色
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//青色
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//蓝色
			break;
		case 7:
			YAxisComponent->SetRotation(frotator());//绿
			XAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));//红色
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//青色
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//蓝色
			break;
		default: break;
		}

	}
}

XMVECTOR GRotateArrow::GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation)
{
	XMFLOAT3 Dir{ 1.f, 1.f, 1.f };
	XMVECTOR OutDirection = XMLoadFloat3(&Dir);
	return OutDirection;
}
