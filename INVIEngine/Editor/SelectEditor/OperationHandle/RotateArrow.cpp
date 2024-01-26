#include "EngineMinimal.h"
#include "RotateArrow.h"

#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Construction/MacroConstruction.h"

extern CMeshComponent* SelectedAxisComponent;	// ��ѡ�е�����
extern GActorObject* SelectedActor;	// ��ѡ�е�����

GRotateArrow::GRotateArrow()
{
	LastT2Value = 0.0f;

	FCreateObjectParams params{};

	params.Owner = this;

	// ����ʵ��
	XAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	YAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	ZAxisComponent = ConstructionObject<CCustomMeshComponent>(params);

	GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_OPERATE);

	GRotateArrow::SetMesh();

	ResetColor();
}

void GRotateArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/Handle/RotateHandleX.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/Handle/RotateHandleY.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/Handle/RotateHandleZ.fbx");

	// ��תģ�ͣ�ʹ���Ϊ��ȷ������ϵָ��
	/*XAxisComponent->SetRotation({ 0.0f, 90.0f, 0.0f });
	YAxisComponent->SetRotation({ -90.0f, 0.0f, 0.0f });*/
}

void GRotateArrow::OnMouseMoved(int x, int y)
{
	GOperationHandle::OnMouseMoved(x, y);

	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		OnMousePressed(x, y);
	}
}

void GRotateArrow::OnMouseLeftDown(int x, int y)
{
	GOperationHandle::OnMouseLeftDown(x, y);

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

void GRotateArrow::OnMousePressed(int x, int y)
{
	if (SelectedActor and SelectedAxisComponent)
	{
		XMVECTOR ActorLocation{};		// �����λ��
		XMVECTOR DragDirection{};		// �����ק����ķ���
		float t = GetMouseMoveDistance(x, y, ActorLocation, DragDirection);
		if (t != 0)
		{
		}
	}
}
