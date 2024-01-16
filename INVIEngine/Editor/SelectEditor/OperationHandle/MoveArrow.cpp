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

	// ����ʵ��
	XAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	YAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	ZAxisComponent = ConstructionObject<CCustomMeshComponent>(params);

	GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_OPERATE);

	// ����ģ��
	GMoveArrow::SetMesh();

	ResetColor();

}

void GMoveArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/MoveArrow.fbx");

	// ��תģ�ͣ�ʹ���Ϊ��ȷ������ϵָ��
	XAxisComponent->SetRotation({ -90.0f, 0.0f, 0.0f });
	YAxisComponent->SetRotation({ 0.0f, 90.0f, 0.0f });

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
}

extern GActorObject* SelectedActor;	// ��ѡ�е�����


void GMoveArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor)
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
			XMVECTOR ActorLocation = XMLoadFloat3(&SelectedActor->GetPosition());

			// �����ק����ķ���
			XMVECTOR DragDirection{};

			// ����ѡ�е����򣬻�ȡ���ߵķ���
			switch (GetSelectedAxis())
			{
			case AXIS_X:
			{
				// �����ק����X�ᣬҲ���������Right����
				XMFLOAT3 RightVector = SelectedActor->GetRightVector();
				DragDirection = XMLoadFloat3(&RightVector);
				break;
			};
			case AXIS_Y:
			{
				// �����ק����Y�ᣬҲ���������Up����
				XMFLOAT3 UpVector = SelectedActor->GetUpVector();
				DragDirection = XMLoadFloat3(&UpVector);
				break;
			};
			case AXIS_Z:
			{
				// �����ק����Z�ᣬҲ���������Forward����
				XMFLOAT3 ForwardVector = SelectedActor->GetForwardVector();
				DragDirection = XMLoadFloat3(&ForwardVector);
				break;
			};
			default: break;
			}

			XMVECTOR WorldDirectionCrossDragDirection = XMVector3Cross(WorldDirection, DragDirection);
			// �����������߷��������Ĳ�˵�ģ
			float len = XMVectorGetX(XMVector3Length(WorldDirectionCrossDragDirection));

			// �������ߵķ��������������ק����ķ����������ཻ���ʱ��
			float t = XMVectorGetX(XMVector3Dot(XMVector3Cross(ActorLocation - WorldOriginPoint, WorldDirection), WorldDirectionCrossDragDirection) / (len * len));

			// ���������ʱ��t�����ߵķ��������������ק����ķ����������ཻ�㣨ģ�ͱ���ק��Ŀ��㣩
			XMVECTOR DragPoint = WorldOriginPoint + WorldDirection * t;

			// ���������λ��
			SelectedActor->SetPosition(DragPoint);
		}
	}
}

