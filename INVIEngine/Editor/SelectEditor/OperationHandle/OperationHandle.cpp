#include "EngineMinimal.h"
#include "OperationHandle.h"

#include "OperationHandleSelectManage.h"
#include "Component/InputComponent.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/QuaternionCamera.h"
#include "Core/World.h"
#include "Material/Core/Material.h"
#include "Misc/RaycastSystemLibrary.h"


FCaptureOnMousesMoveDelegate MouseMoveDelegate;
FCaptureOnLMousesButtonDownDelegate LeftMouseDownDelegate;	// �������¼�
FCaptureOnLMousesButtonUpDelegate LeftMouseUpDelegate;		// ������̧���¼�
FCaptureOnRMousesButtonDownDelegate RightMouseDownDelegate;	// �Ҽ�����¼�
FCaptureOnRMousesButtonUpDelegate RightMouseUpDelegate;		// �Ҽ����̧���¼�

extern CMeshComponent* SelectedAxisComponent;	// ��ѡ�е�����
extern GActorObject* SelectedActor;	// ��ѡ�е�����

GOperationHandle::GOperationHandle()
{
	FCreateObjectParams params{};
	params.Owner = this;

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
	// InputComponent->OnMouseWheelDelegate.Bind(this, &GOperationHandle::OnMouseMoved);
	// �󶨼�������¼�
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &GOperationHandle::ExecuteInput);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GOperationHandle::OnMouseMoved);

	InputComponent->OnMouseLeftDownDelegate.Bind(this, &GOperationHandle::OnMouseLeftDown);
	InputComponent->OnMouseLeftUpDelegate.Bind(this, &GOperationHandle::OnMouseLeftUp);

	// ����ʵ��
	XAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	YAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	ZAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	AnyAxisComponent = ConstructionObject<CCustomMeshComponent>(params);

	GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_OPERATE);

}

void GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer)
{
	XAxisComponent->SetRenderLayerType(mesh_render_layer);
	YAxisComponent->SetRenderLayerType(mesh_render_layer);
	ZAxisComponent->SetRenderLayerType(mesh_render_layer);
	AnyAxisComponent->SetRenderLayerType(mesh_render_layer);
}

void GOperationHandle::ResetColor()
{
	ResetColor(XAxisComponent, {1.f, 0.f, 0.f, 1.f});
	ResetColor(YAxisComponent, {0.f, 1.f, 0.f, 1.f});
	ResetColor(ZAxisComponent, {0.f, 0.f, 1.f, 1.f});
	ResetColor(AnyAxisComponent, {0.f, 1.f, 1.f, 1.f});
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

void GOperationHandle::BeginInit()
{
	GActorObject::BeginInit();

	SetVisible(false);
}

void GOperationHandle::Tick(float DeltaTime)
{
	GActorObject::Tick(DeltaTime);

	if (GetWorld())
	{
		if (GetWorld()->GetQuaternionCamera())
		{
			// �����λ��
			fvector_3d New3Value = EngineMath::ToVector3d(GetWorld()->GetQuaternionCamera()->GetPosition()) - EngineMath::ToVector3d(GetPosition());
			fvector_3d Scale{ New3Value.len() / FixedZoom };

			// ENGINE_LOG("zoom = %f, %f, %f, %f, %f", New3Value.len(), FixedZoom, Scale.x, Scale.y, Scale.z);

			XMFLOAT3 NewScale = EngineMath::ToFloat3(Scale);
			SetScale(NewScale);

		}
	}
}

void GOperationHandle::SetPosition(const XMVECTOR& InNewPosition)
{
	// GActorObject::SetPosition(InNewPosition);
	XMFLOAT3 position;
	XMStoreFloat3(&position, InNewPosition);
	SetPosition(position);
}

void GOperationHandle::SetPosition(const XMFLOAT3& InNewPosition)
{
	// GActorObject::SetPosition(InNewPosition);
	XAxisComponent->SetPosition(InNewPosition);
	YAxisComponent->SetPosition(InNewPosition);
	ZAxisComponent->SetPosition(InNewPosition);
	AnyAxisComponent->SetPosition(InNewPosition);
}

void GOperationHandle::SetVisible(bool visible)
{
	XAxisComponent->SetVisible(visible);
	YAxisComponent->SetVisible(visible);
	ZAxisComponent->SetVisible(visible);
	AnyAxisComponent->SetVisible(visible);
}

void GOperationHandle::SetVisible(bool visible, CCustomMeshComponent* axis_component)
{
	if (axis_component)
	{
		axis_component->SetVisible(visible);
	}
}

void GOperationHandle::SetScale(const XMFLOAT3& InNewScale)
{
	GActorObject::SetScale(InNewScale);
	fvector_3d NewScale = EngineMath::ToVector3d(InNewScale);

	if (NewScale >= fvector_3d(0.5f))
	{
		if (XAxisComponent && YAxisComponent && ZAxisComponent)
		{
			XAxisComponent->SetScale(InNewScale);
			YAxisComponent->SetScale(InNewScale);
			ZAxisComponent->SetScale(InNewScale);
			AnyAxisComponent->SetScale(InNewScale);
		}
	}

}

bool GOperationHandle::IsVisible()
{
	return XAxisComponent->IsVisible();
}

ESelectedAxis GOperationHandle::GetSelectedAxis() const
{
	if (SelectedAxisComponent)
	{
		if (SelectedAxisComponent == XAxisComponent)
		{
			return AXIS_X;
		}
		else if (SelectedAxisComponent == YAxisComponent)
		{
			return AXIS_Y;
		}
		else if (SelectedAxisComponent == ZAxisComponent)
		{
			return AXIS_Z;
		}
		else if (SelectedAxisComponent == AnyAxisComponent)
		{
			return AXIS_ANY;
		}
		else
		{
			return AXIS_NONE;
		}
	}
	return AXIS_NONE;
}

void GOperationHandle::OnMouseMoved(int x, int y)
{
	if (!IsCurrentSelectedHandle()) return;		// ������ǵ�ǰѡ�еĲ���������Ͳ����в���
	if (SelectedAxisComponent) return;			// ���ѡ�е�����Ϊ�գ��Ͳ����в���
	
	XMFLOAT2 mousePos(x, y);

	EngineType::FHitResult HitResult{};
	FRayCastSystemLibrary::CheckObjectIsSelected(GetWorld(), mousePos, this, HitResult);

	if (HitResult.bHit)
	{
		ResetColor();
		if (HitResult.HitComponent == XAxisComponent)
		{
			ResetColor(XAxisComponent, { 1.f, 0.95f, 0.f, 1.f });
		}
		else if (HitResult.HitComponent == YAxisComponent)
		{
			ResetColor(YAxisComponent, { 1.f, 0.95f, 0.f, 1.f });
		}
		else if (HitResult.HitComponent == ZAxisComponent)
		{
			ResetColor(ZAxisComponent, { 1.f, 0.95f, 0.f, 1.f });
		}
		else if (HitResult.HitComponent == AnyAxisComponent)
		{
			ResetColor(AnyAxisComponent, { 1.f, 0.95f, 0.f, 1.f });
		}
	}
	else
	{
		if (SelectedAxisComponent)
		{
			ResetColor(dynamic_cast<CCustomMeshComponent*>(SelectedAxisComponent), { 1.f, 0.95f, 0.f, 1.f });
		}
		else
		{
			ResetColor();
		}
	}
}

void GOperationHandle::OnMouseLeftDown(int x, int y)
{
	if (SelectedActor)
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
}

void GOperationHandle::OnMouseLeftUp(int x, int y)
{
	SelectedAxisComponent = nullptr;
}

float GOperationHandle::GetMouseMoveDistance(int x, int y, XMVECTOR& ActorLocation, XMVECTOR& DragDirection)
{
	XMVECTOR ViewOriginPoint{};		// �ӿ�������ԭ�㣨����Ļ���꣩
	XMVECTOR ViewDirection{};		// �ӿ������߷���
	XMMATRIX InverseViewMatrix{};	// �ӿڱ任����������

	if (FRayCastSystemLibrary::GetRayCastParamByScreen(GetWorld(), XMFLOAT2(x, y), ViewOriginPoint, ViewDirection, InverseViewMatrix))
	{
		// ��ȡ����ռ��µ����߲���
		XMVECTOR WorldOriginPoint = XMVector3TransformCoord(ViewOriginPoint, InverseViewMatrix);
		XMVECTOR WorldDirection = XMVector3TransformNormal(ViewDirection, InverseViewMatrix);

		// �Է�����й�һ��
		WorldDirection = XMVector3Normalize(WorldDirection);

		// ��ȡѡ�������λ��
		ActorLocation = XMLoadFloat3(&SelectedActor->GetPosition());

		// �����ק����ķ���

		// ����ѡ�е����򣬻�ȡ���ߵķ���
		switch (GetSelectedAxis())
		{
		case AXIS_X:
		{
			// �����ק����X�ᣬҲ���������Right����
			XMFLOAT3 RightVector = IsWorldOperate() ? XMFLOAT3{ 1.0f, 0.0f, 0.0f } : SelectedActor->GetRightVector();
			DragDirection = XMLoadFloat3(&RightVector);
			break;
		};
		case AXIS_Y:
		{
			// �����ק����Y�ᣬҲ���������Up����
			XMFLOAT3 UpVector = IsWorldOperate() ? XMFLOAT3{ 0.0f, 1.0f, 0.0f } : SelectedActor->GetUpVector();
			DragDirection = XMLoadFloat3(&UpVector);
			break;
		};
		case AXIS_Z:
		{
			// �����ק����Z�ᣬҲ���������Forward����
			XMFLOAT3 ForwardVector = IsWorldOperate() ? XMFLOAT3{ 0.0f, 0.0f, 1.0f } : SelectedActor->GetForwardVector();
			DragDirection = XMLoadFloat3(&ForwardVector);
			break;
		};
		default: break;
		}

		XMVECTOR WorldDirectionCrossDragDirection = XMVector3Cross(WorldDirection, DragDirection);
		// �����������߷��������Ĳ�˵�ģ
		float len = XMVectorGetX(XMVector3Length(WorldDirectionCrossDragDirection));

		// �������ߵķ��������������ק����ķ����������ཻ���ʱ��
		const float t = XMVectorGetX(XMVector3Dot(XMVector3Cross(ActorLocation - WorldOriginPoint, WorldDirection), WorldDirectionCrossDragDirection) / (len * len));

		return t;
	}

	return 0.0f;
}

bool GOperationHandle::IsCurrentSelectedHandle()
{
	return FOperationHandleSelectManage::Get()->GetSelectedOperationHandle() == this;
}
