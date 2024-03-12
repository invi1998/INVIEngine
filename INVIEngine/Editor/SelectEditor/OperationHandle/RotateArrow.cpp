#include "EngineMinimal.h"
#include "RotateArrow.h"

#include "OperationHandleSelectManage.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/PlaneMeshComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/QuaternionCamera.h"
#include "Core/World.h"
#include "Core/Construction/MacroConstruction.h"
#include "Material/Core/Material.h"
#include "Misc/RaycastSystemLibrary.h"

extern CMeshComponent* SelectedAxisComponent;	// 被选中的轴向
extern GActorObject* SelectedActor;	// 被选中的物体

GRotateArrow::GRotateArrow()
	: GOperationHandle()
{
	FCreateObjectParams Params;
	Params.Owner = this;

	XPlaneComponent = ConstructionObject<CPlaneMeshComponent>(Params);
	YPlaneComponent = ConstructionObject<CPlaneMeshComponent>(Params);
	ZPlaneComponent = ConstructionObject<CPlaneMeshComponent>(Params);

	// 将面片添加进射线检测的忽略列表
	AddIgnoreComponent(XPlaneComponent);
	AddIgnoreComponent(YPlaneComponent);
	AddIgnoreComponent(ZPlaneComponent);

	// 设置渲染层
	XPlaneComponent->SetRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_ROT_PLANE);
	YPlaneComponent->SetRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_ROT_PLANE);
	ZPlaneComponent->SetRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_ROT_PLANE);

	GRotateArrow::SetMesh();

	ResetColor();
}

void GRotateArrow::SetMesh()
{
	// 创建操作手柄的网格
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/Handle/RotateHandleX.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/Handle/RotateHandleY.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/Handle/RotateHandleZ.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, AnyAxisComponent, "Asserts/Mesh/Handle/RotateHandleZ.fbx");

	// 创建轴平面（CD面片，用于显示轴的平面）
	CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, XPlaneComponent, 5.2f, 5.2f, 2, 2);	// 创建x轴的平面，宽高为5.2，细分为2
	CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, YPlaneComponent, 5.2f, 5.2f, 2, 2);	// 创建y轴的平面，宽高为5.2，细分为2
	CREATE_RENDER_DATA_BY_COMPONENT(CPlaneMeshComponent, ZPlaneComponent, 5.2f, 5.2f, 2, 2);	// 创建z轴的平面，宽高为5.2，细分为2

	// 让平面相互垂直
	XPlaneComponent->SetRotation(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
	YPlaneComponent->SetRotation(XMFLOAT3{ 90.0f, 0.0f, 0.0f });
	ZPlaneComponent->SetRotation(XMFLOAT3{ 0.0f, 0.0f, -90.0f });

	// 给平面添加材质
	SetMaterial(XPlaneComponent, "Asserts/Texture/Rot_Handle_Plane.dds");
	SetMaterial(ZPlaneComponent, "Asserts/Texture/Rot_Handle_Plane.dds");
	SetMaterial(YPlaneComponent, "Asserts/Texture/Rot_Handle_Plane.dds");

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
		ResetCDValue();

		XMFLOAT2 mousePos(x, y);

		EngineType::FHitResult HitResult{};
		FRayCastSystemLibrary::CheckObjectIsSelected(GetWorld(), mousePos, this, HitResult, GetIgnoreComponents());

		if (HitResult.bHit)
		{
			if (HitResult.HitComponent == XAxisComponent)
			{
				SelectedAxisComponent = XAxisComponent;
				SetVisible(false, YAxisComponent);
				SetVisible(false, ZAxisComponent);
				SetVisible(false, AnyAxisComponent);
				SetVisible(true, XAxisComponent);
			}
			else if (HitResult.HitComponent == YAxisComponent)
			{
				SelectedAxisComponent = YAxisComponent;
				SetVisible(false, XAxisComponent);
				SetVisible(false, ZAxisComponent);
				SetVisible(false, AnyAxisComponent);
				SetVisible(true, YAxisComponent);
			}
			else if (HitResult.HitComponent == ZAxisComponent)
			{
				SelectedAxisComponent = ZAxisComponent;
				SetVisible(false, YAxisComponent);
				SetVisible(false, XAxisComponent);
				SetVisible(false, AnyAxisComponent);
				SetVisible(true, ZAxisComponent);
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
			RotateRadio = 0.f;
		}
	}
}

void GRotateArrow::OnMouseLeftUp(int x, int y)
{
	GOperationHandle::OnMouseLeftUp(x, y);

	if (SelectedActor && IsCurrentSelectedHandle())
	{
		SetVisible();
	}
}

void GRotateArrow::ExecuteInput()
{
	if (SelectedActor)
	{
		if (FInput::IsKeyPressed(Key::E))
		{
			FOperationHandleSelectManage::Get()->DisplaySelectedHandle(this);

			SetVisible();
		}
	}
}

float GetSymbol(float value, bool flip = false)
{
	if (flip)
	{
		return value < 0 ? -1.0f : 1.0f;
	}
	else
	{
		return value < 0 ? 1.0f : -1.0f;
	}

	return 0.f;
}

void GRotateArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor && SelectedAxisComponent && IsCurrentSelectedHandle())
	{
		XMVECTOR ActorLocation{};		// 物体的位置
		XMVECTOR DragDirection{};		// 鼠标拖拽的轴的方向
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);

		XMVECTOR tempNull{};

		if (t != 0)
		{
			GOperationHandle::GetSelectedObjectDirection(tempNull, tempNull, tempNull, DragDirection);

			float CameraAndSelectedActorDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&GetWorld()->GetQuaternionCamera()->GetPosition()) - XMLoadFloat3(&SelectedActor->GetPosition())));

			float Delta = (t - LastT2Value) / CameraAndSelectedActorDistance;	// 之所以要除以摄像机和物体的距离，是因为我们需要将鼠标拖拽的距离转换为旋转的角度，我们希望摄像机和物体的距离越远，旋转的角度越小

			// 获取鼠标拖拽的旋转间距（角度）
			float offset = Delta - RotateRadio;
			float symbol = GetSymbolByCubeIndex(offset);

			// TODO:这里先用simple_library库的四元数，后续考虑使用DX12库的四元数
			// 定义旋转
			fvector_3d deltaVector;
			XMFLOAT3 ActorDirFloat3;
			XMStoreFloat3(&ActorDirFloat3, DragDirection);
			fvector_3d dragDirection = EngineMath::ToVector3d(ActorDirFloat3);
			deltaVector = dragDirection * symbol * fabsf(offset) * 360.f;	// 旋转的角度

			// 获取物体的欧拉角
			frotator rotationRotator = SelectedActor->GetRotationFrotator();

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

			XMFLOAT3 resultRotation{ newRotationRotator.roll, newRotationRotator.pitch, newRotationRotator.yaw };

			if (IsWorldOperate())
			{
				XMMATRIX WorldInverseMatrix{};
				EngineMath::BuildInverseMatrix(WorldInverseMatrix, SelectedActor->GetPosition(), SelectedActor->GetScale(), SelectedActor->GetRightVector(), SelectedActor->GetUpVector(), SelectedActor->GetForwardVector());	// 物体的世界矩阵的逆矩阵

				FXMVECTOR ResultRotation = XMLoadFloat3(&resultRotation);	// 新的旋转角度

				XMVECTOR ResultRotationInWorld = XMVector3TransformNormal(ResultRotation, WorldInverseMatrix);	// 新的旋转角度转换为世界坐标系下的旋转角度

				XMStoreFloat3(&resultRotation, ResultRotationInWorld);
			}

			SelectedActor->SetRotation(resultRotation);

			RotateRadio = Delta;
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

		XPlaneComponent->SetScale(InNewScale);
		YPlaneComponent->SetScale(InNewScale);
		ZPlaneComponent->SetScale(InNewScale);
	}
}

void GRotateArrow::SetPosition(const XMFLOAT3& InNewPosition)
{
	GOperationHandle::SetPosition(InNewPosition);

	XPlaneComponent->SetPosition(InNewPosition);
	YPlaneComponent->SetPosition(InNewPosition);
	ZPlaneComponent->SetPosition(InNewPosition);
}

void GRotateArrow::SetVisible()
{
	GOperationHandle::SetVisible();

	XPlaneComponent->SetVisible(false);
	YPlaneComponent->SetVisible(false);
	ZPlaneComponent->SetVisible(false);
}

void GRotateArrow::SetVisible(bool visible)
{
	XPlaneComponent->SetVisible(false);
	YPlaneComponent->SetVisible(false);
	ZPlaneComponent->SetVisible(false);

	GOperationHandle::SetVisible(visible);
}

void GRotateArrow::SetVisible(bool visible, CCustomMeshComponent* axis_component)
{
	GOperationHandle::SetVisible(visible, axis_component);

	switch (GetSelectedAxis())
	{
	case AXIS_NONE: break;
	case AXIS_X: {
		ZPlaneComponent->SetVisible(visible);
		break;
	}
	case AXIS_Y: {
		XPlaneComponent->SetVisible(visible);
		break;
	}
	case AXIS_Z: {
		YPlaneComponent->SetVisible(visible);
		break;
	}
	case AXIS_ANY: break;
	default: break;
	}
}

void GRotateArrow::Tick(float DeltaTime)
{
	GOperationHandle::Tick(DeltaTime);

	if (IsCurrentSelectedHandle())
	{
		// 我们需要将摄像机的位置转换为操作手柄的坐标系下的位置
		XMMATRIX ArrowMatrixInverse{};
		EngineMath::BuildInverseMatrix(ArrowMatrixInverse, GetPosition(), GetScale(), GetRightVector(), GetUpVector(), GetForwardVector());	// 操作手柄的世界矩阵的逆矩阵

		XMVECTOR CameraLocation = XMLoadFloat3(&GetWorld()->GetQuaternionCamera()->GetPosition());	// 摄像机的位置

		XMVECTOR ArrowMatrixInverseLocation = XMVector3TransformCoord(CameraLocation, ArrowMatrixInverse);	// 摄像机的位置转换为操作手柄的坐标系下的位置

		XMFLOAT3 ArrowMatrixInverseLocationFloat3{};
		XMStoreFloat3(&ArrowMatrixInverseLocationFloat3, ArrowMatrixInverseLocation);

		ENGINE_LOG_ERROR("x = %f, y = %f, z = %f", ArrowMatrixInverseLocationFloat3.x, ArrowMatrixInverseLocationFloat3.y, ArrowMatrixInverseLocationFloat3.z);

		Sample8CubeIndex = EngineMath::GetSample8CubeIndex(EngineMath::ToVector3d(ArrowMatrixInverseLocationFloat3));

		switch (Sample8CubeIndex)
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

void GRotateArrow::SetCDValue(float InCDValue)
{
	SetCDValue(InCDValue, XPlaneComponent);
	SetCDValue(InCDValue, YPlaneComponent);
	SetCDValue(InCDValue, ZPlaneComponent);
}

void GRotateArrow::SetCDValue(float InCDValue, CMeshComponent* axis_component)
{
	if (CMaterial* material = (*axis_component->GetMaterial())[0])
	{
		material->SetParam(0, InCDValue);
	}
}

void GRotateArrow::ResetCDValue()
{
	SetCDValue(0.0f);
}

XMVECTOR GRotateArrow::GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation)
{
	XMFLOAT3 Dir{ 1.f, 1.f, 1.f };
	XMVECTOR OutDirection = XMLoadFloat3(&Dir);
	return OutDirection;
}

void GRotateArrow::SetMaterial(CMeshComponent* axis_component, const std::string& path)
{
	if (CMaterial* material = (*axis_component->GetMaterial())[0])
	{
		material->SetBaseColorIndexKey(path);
	}
}

void GRotateArrow::GetSelectedObjectDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection,
	XMVECTOR& ActorLocation, XMVECTOR& DragDirection)
{
	if (IsWorldOperate())
	{
		// 世界坐标系下获取方向
		switch (GetSelectedAxis())
		{
		case AXIS_X:
		{
			// 鼠标拖拽的是X轴，也就是物体的Right方向
			XMFLOAT3 DirVector = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
			DragDirection = XMLoadFloat3(&DirVector);
			break;
		};
		case AXIS_Y:
		{
			// 鼠标拖拽的是Y轴，也就是物体的Up方向
			XMFLOAT3 DirVector = XMFLOAT3{ 1.0f, 0.0f, 0.0f };
			DragDirection = XMLoadFloat3(&DirVector);
			break;
		};
		case AXIS_Z:
		{
			// 鼠标拖拽的是Z轴，也就是物体的Forward方向
			XMFLOAT3 DirVector = XMFLOAT3{ 1.0f, 0.0f, 0.0f };
			DragDirection = XMLoadFloat3(&DirVector);
			break;
		};
		case AXIS_ANY:
		{
			// 鼠标拖拽的是任意轴
			DragDirection = GetAnyAxisDirection(WorldOriginPoint, WorldDirection, ActorLocation);
			break;
		}
		default: break;
		}
	}
	else
	{
		// 根据选中的轴向，获取射线的方向
		switch (GetSelectedAxis())
		{
		case AXIS_X:
		{
			// 鼠标拖拽的是X轴，也就是物体的Right方向
			XMFLOAT3 DirVector = SelectedActor->GetUpVector();
			DragDirection = XMLoadFloat3(&DirVector);
			break;
		};
		case AXIS_Y:
		{
			// 鼠标拖拽的是Y轴，也就是物体的Up方向
			XMFLOAT3 DirVector = SelectedActor->GetRightVector();
			DragDirection = XMLoadFloat3(&DirVector);
			break;
		};
		case AXIS_Z:
		{
			// 鼠标拖拽的是Z轴，也就是物体的Forward方向
			XMFLOAT3 DirVector = SelectedActor->GetForwardVector();
			DragDirection = XMLoadFloat3(&DirVector);
			break;
		};
		case AXIS_ANY:
		{
			// 鼠标拖拽的是任意轴
			DragDirection = GetAnyAxisDirection(WorldOriginPoint, WorldDirection, ActorLocation);
			break;
		}
		default: break;
		}
	}
}

float GRotateArrow::GetSymbolByCubeIndex(float offset)
{
	float symbol = 1.f;
	// 获取轴向
	ESelectedAxis SelectedAxis = GetSelectedAxis();

	switch (Sample8CubeIndex)
	{
	case 0:
	case 1:
	{
		switch (SelectedAxis)
		{
		case AXIS_X:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_Y:
		{
			symbol = GetSymbol(offset, true);
			break;
		}
		case AXIS_Z:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_ANY:
		{
			symbol = GetSymbol(offset);
			break;
		}
		default: break;
		}
		break;
	}
	case 2:
	case 3:
	{
		switch (SelectedAxis)
		{
		case AXIS_X:
		{
			symbol = GetSymbol(offset, true);
			break;
		}
		case AXIS_Y:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_Z:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_ANY:
		{
			symbol = GetSymbol(offset);
			break;
		}
		default: break;
		}
		break;
	}
	case 4:
	case 5:
	{
		switch (SelectedAxis)
		{
		case AXIS_X:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_Y:
		{
			symbol = GetSymbol(offset, true);
			break;
		}
		case AXIS_Z:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_ANY:
		{
			symbol = GetSymbol(offset);
			break;
		}
		default: break;
		}
		break;
	}
	case 6:
	case 7:
	{
		switch (SelectedAxis)
		{
		case AXIS_X:
		{
			symbol = GetSymbol(offset, true);
			break;
		}
		case AXIS_Y:
		{
			symbol = GetSymbol(offset);
			break;
		}
		case AXIS_Z:
		{
			symbol = GetSymbol(offset, true);
			break;
		}
		case AXIS_ANY:
		{
			symbol = GetSymbol(offset);
			break;
		}
		default: break;
		}
		break;
	}
	}

	return symbol;
}
