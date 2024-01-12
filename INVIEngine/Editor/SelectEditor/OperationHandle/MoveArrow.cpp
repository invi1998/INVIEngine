#include "EngineMinimal.h"
#include "MoveArrow.h"

#include "Collision/CollisionSceneQuery.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"
#include "Misc/RaycastSystemLibrary.h"

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
	XAxisComponent->SetRotation({ -90.0f, 0.0f, 0.0f });
	YAxisComponent->SetRotation({ 0.0f, 90.0f, 0.0f });

	// ResetColor();

}

extern GActorObject* SelectedActor;	// 被选中的物体
void GMoveArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);

	if (SelectedActor)
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
			XMVECTOR ActorLocation = XMLoadFloat3(&SelectedActor->GetPosition());

			// 根据选中的轴向，获取射线的方向
		}
	}

	
}

