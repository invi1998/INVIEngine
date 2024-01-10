#include "EngineMinimal.h"
#include "MoveArrow.h"

#include "Component/Mesh/CustomMeshComponent.h"
#include "Core/Construction/MacroConstruction.h"

GMoveArrow::GMoveArrow()
{
	FCreateObjectParams params{};

	params.Owner = this;

	// ����ʵ��
	XAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	YAxisComponent = ConstructionObject<CCustomMeshComponent>(params);
	ZAxisComponent = ConstructionObject<CCustomMeshComponent>(params);

	// ����ģ��
	GMoveArrow::SetMesh();

	ResetColor();

	GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_OPERATE);
}

void GMoveArrow::SetMesh()
{
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, XAxisComponent, "Asserts/Mesh/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, YAxisComponent, "Asserts/Mesh/MoveArrow.fbx");
	CREATE_RENDER_DATA_BY_COMPONENT(CCustomMeshComponent, ZAxisComponent, "Asserts/Mesh/MoveArrow.fbx");

	// ��תģ�ͣ�ʹ���Ϊ��ȷ������ϵָ��
	XAxisComponent->SetRotation({ -90.0f, 0.0f, 0.0f });
	YAxisComponent->SetRotation({ 0.0f, 90.0f, 0.0f });

}
