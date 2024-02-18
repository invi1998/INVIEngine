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
FCaptureOnLMousesButtonDownDelegate LeftMouseDownDelegate;	// 左键鼠标事件
FCaptureOnLMousesButtonUpDelegate LeftMouseUpDelegate;		// 左键鼠标抬起事件
FCaptureOnRMousesButtonDownDelegate RightMouseDownDelegate;	// 右键鼠标事件
FCaptureOnRMousesButtonUpDelegate RightMouseUpDelegate;		// 右键鼠标抬起事件

extern CMeshComponent* SelectedAxisComponent;	// 被选中的轴向
extern GActorObject* SelectedActor;	// 被选中的物体

GOperationHandle::GOperationHandle()
{
	FCreateObjectParams params{};
	params.Owner = this;

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
	// InputComponent->OnMouseWheelDelegate.Bind(this, &GOperationHandle::OnMouseMoved);
	// 绑定键盘鼠标事件
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &GOperationHandle::ExecuteInput);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GOperationHandle::OnMouseMoved);

	InputComponent->OnMouseLeftDownDelegate.Bind(this, &GOperationHandle::OnMouseLeftDown);
	InputComponent->OnMouseLeftUpDelegate.Bind(this, &GOperationHandle::OnMouseLeftUp);

	// 创建实例
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
			// 摄像机位置
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
	if (!IsCurrentSelectedHandle()) return;		// 如果不是当前选中的操作句柄，就不进行操作
	if (SelectedAxisComponent) return;			// 如果选中的轴向不为空，就不进行操作
	
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
	XMVECTOR ViewOriginPoint{};		// 视口下射线原点（非屏幕坐标）
	XMVECTOR ViewDirection{};		// 视口下射线方向
	XMMATRIX InverseViewMatrix{};	// 视口变换矩阵的逆矩阵

	if (FRayCastSystemLibrary::GetRayCastParamByScreen(GetWorld(), XMFLOAT2(x, y), ViewOriginPoint, ViewDirection, InverseViewMatrix))
	{
		// 获取世界空间下的射线参数
		XMVECTOR WorldOriginPoint = XMVector3TransformCoord(ViewOriginPoint, InverseViewMatrix);
		XMVECTOR WorldDirection = XMVector3TransformNormal(ViewDirection, InverseViewMatrix);

		// 对方向进行归一化
		WorldDirection = XMVector3Normalize(WorldDirection);

		// 获取选中物体的位置
		ActorLocation = XMLoadFloat3(&SelectedActor->GetPosition());

		// 鼠标拖拽的轴的方向

		// 根据选中的轴向，获取射线的方向
		switch (GetSelectedAxis())
		{
		case AXIS_X:
		{
			// 鼠标拖拽的是X轴，也就是物体的Right方向
			XMFLOAT3 RightVector = IsWorldOperate() ? XMFLOAT3{ 1.0f, 0.0f, 0.0f } : SelectedActor->GetRightVector();
			DragDirection = XMLoadFloat3(&RightVector);
			break;
		};
		case AXIS_Y:
		{
			// 鼠标拖拽的是Y轴，也就是物体的Up方向
			XMFLOAT3 UpVector = IsWorldOperate() ? XMFLOAT3{ 0.0f, 1.0f, 0.0f } : SelectedActor->GetUpVector();
			DragDirection = XMLoadFloat3(&UpVector);
			break;
		};
		case AXIS_Z:
		{
			// 鼠标拖拽的是Z轴，也就是物体的Forward方向
			XMFLOAT3 ForwardVector = IsWorldOperate() ? XMFLOAT3{ 0.0f, 0.0f, 1.0f } : SelectedActor->GetForwardVector();
			DragDirection = XMLoadFloat3(&ForwardVector);
			break;
		};
		default: break;
		}

		XMVECTOR WorldDirectionCrossDragDirection = XMVector3Cross(WorldDirection, DragDirection);
		// 计算两个射线方向向量的叉乘的模
		float len = XMVectorGetX(XMVector3Length(WorldDirectionCrossDragDirection));

		// 计算射线的方向向量和鼠标拖拽的轴的方向向量的相交点的时间
		const float t = XMVectorGetX(XMVector3Dot(XMVector3Cross(ActorLocation - WorldOriginPoint, WorldDirection), WorldDirectionCrossDragDirection) / (len * len));

		return t;
	}

	return 0.0f;
}

bool GOperationHandle::IsCurrentSelectedHandle()
{
	return FOperationHandleSelectManage::Get()->GetSelectedOperationHandle() == this;
}
