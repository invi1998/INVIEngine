#include "EngineMinimal.h"
#include "MoveArrow.h"

#include "Collision/CollisionSceneQuery.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"
#include "Misc/RaycastSystemLibrary.h"

extern CMeshComponent* SelectedAxisComponent;	// 被选中的轴向

GMoveArrow::GMoveArrow()
{
	FCreateObjectParams params{};

	params.Owner = this;

	// 创建实例
	XAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	YAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	ZAxisComponent = ConstructionObject<CCustomMeshComponent>(params);

	GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_OPERATE);

	// 设置模型
	GMoveArrow::SetMesh();

	ResetColor();

}

void GMoveArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/MoveArrow.fbx");

	// 旋转模型，使其成为正确的坐标系指向
	XAxisComponent->SetRotation({ 0.0f, 90.0f, 0.0f });
	YAxisComponent->SetRotation({ -90.0f, 0.0f, 0.0f });

	// ResetColor();

}

void GMoveArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);

	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		OnMousePressed(x, y);
	}
}

void GMoveArrow::OnMouseLeftDown(int x, int y)
{
	GOperationHandle::OnMouseLeftDown(x, y);

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

extern GActorObject* SelectedActor;	// 被选中的物体


void GMoveArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor and SelectedAxisComponent)
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

float GMoveArrow::GetMouseMoveDistance(int x, int y, XMVECTOR& ActorLocation, XMVECTOR& DragDirection)
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

