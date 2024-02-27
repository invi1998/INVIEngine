#include "EngineMinimal.h"
#include "RotateArrow.h"

#include "OperationHandleSelectManage.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/QuaternionCamera.h"
#include "Core/World.h"
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
			// ��ȡ�������ת
			XMFLOAT3 ActorRotation = SelectedActor->GetRotation();
			fvector_3d rotation = EngineMath::ToVector3d(ActorRotation);

			// ��ȡ�����ק����ת��ࣨ�Ƕȣ�
			float angle = 0.0f;
			float offset = t - LastT2Value;
			angle = offset < 0 ? -2.25f : 2.25f;

			// TODO:��������simple_library�����Ԫ�������������Լ�ʵ��
			// ������ת
			fvector_3d deltaVector;
			XMFLOAT3 ActorDirFloat3;
			XMStoreFloat3(&ActorDirFloat3, DragDirection);
			fvector_3d dragDirection = EngineMath::ToVector3d(ActorDirFloat3);
			deltaVector = dragDirection * angle;

			// ���������תת��Ϊŷ���ǽṹ
			frotator rotationRotator(rotation.y, rotation.z, rotation.x);

			// �����ǵ���ת�Ƕ�ҲתΪŷ���ǽṹ
			frotator deltaRotator(deltaVector.y, deltaVector.z, deltaVector.x);

			// �������ŷ���ǽṹת��Ϊ��Ԫ��
			fquat rotationQuat;
			// �����ǵ���ת�Ƕ�ҲתΪ��Ԫ��
			fquat deltaQuat;

			// ��ʱ���ǵõ���������Ĺ�������ϵ�µ���ת��Ԫ������������ϵ�µ���ת��Ԫ������������Ҫ����ת��Ϊ��������ϵ�µ���ת��Ԫ��
			rotationQuat.inertia_to_object(rotationRotator);
			deltaQuat.inertia_to_object(deltaRotator);

			// ���������ת��Ԫ���ͼ��������ת��Ԫ����ˣ��õ��µ���ת��Ԫ��������µ���ת��Ԫ������������ϵ�µ�
			fquat newRotationQuat = rotationQuat * deltaQuat;

			// ���µ���ת��Ԫ��ת��Ϊŷ���ǣ������������ת��ע����Ҫ����������ϵ�µ���ת��Ԫ��ת��Ϊ��������ϵ�µ���ת��Ԫ����
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
		// ������Ҫ���������λ��ת��Ϊ�����ֱ�������ϵ�µ�λ��
		XMFLOAT4X4 ArrowMatrix{};
		EngineMath::BuildMatrix(ArrowMatrix, GetPosition(), GetScale(), GetRightVector(), GetUpVector(), GetForwardVector());

		XMMATRIX ArrowMatrixXM = XMLoadFloat4x4(&ArrowMatrix);		// �����ֱ����������
		XMVECTOR ArrowMatrixDetemine = XMMatrixDeterminant(ArrowMatrixXM);	// �����ֱ���������������ʽs
		XMMATRIX ArrowMatrixInverse = XMMatrixInverse(&ArrowMatrixDetemine, ArrowMatrixXM);	// �����ֱ����������������

		XMVECTOR CameraLocation = XMLoadFloat3(&GetWorld()->GetQuaternionCamera()->GetPosition());	// �������λ��

		XMVECTOR ArrowMatrixInverseLocation = XMVector3TransformCoord(CameraLocation, ArrowMatrixInverse);	// �������λ��ת��Ϊ�����ֱ�������ϵ�µ�λ��

		XMFLOAT3 ArrowMatrixInverseLocationFloat3;
		XMStoreFloat3(&ArrowMatrixInverseLocationFloat3, ArrowMatrixInverseLocation);

		int type = EngineMath::GetSample8CubeIndex(EngineMath::ToVector3d(ArrowMatrixInverseLocationFloat3));

		switch (type)
		{
		case 0:
			YAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-90.f, 0, 0.f));//��ɫ
			AnyAxisComponent->SetRotation(frotator());
			ZAxisComponent->SetRotation(frotator());
			break;
		case 1:
			YAxisComponent->SetRotation(frotator(0.f, -180.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-90.f, 0, 0.f));//��ɫ
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
			YAxisComponent->SetRotation(frotator());//��
			XAxisComponent->SetRotation(frotator());//��ɫ
			AnyAxisComponent->SetRotation(frotator());//��ɫ
			ZAxisComponent->SetRotation(frotator());//��ɫ
			break;
		case 4:
			YAxisComponent->SetRotation(frotator(0.f, 90.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-180.f, 0, 0.f));//��ɫ
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			break;
		case 5:
			YAxisComponent->SetRotation(frotator(0.f, 180.f, 0.f));//��
			XAxisComponent->SetRotation(frotator(-180.f, 0, 0.f));//��ɫ
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			break;
		case 6:
			YAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��
			XAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));//��ɫ
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, -180.f));//��ɫ
			break;
		case 7:
			YAxisComponent->SetRotation(frotator());//��
			XAxisComponent->SetRotation(frotator(0.f, 0.f, 180.f));//��ɫ
			AnyAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
			ZAxisComponent->SetRotation(frotator(0.f, 0.f, 90.f));//��ɫ
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
