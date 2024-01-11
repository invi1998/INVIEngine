#include "EngineMinimal.h"
#include "OperationHandle.h"

#include "Component/InputComponent.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Material/Core/Material.h"
#include "Misc/RaycastSystemLibrary.h"


FCaptureOnMousesMoveDelegate MouseMoveDelegate;

GOperationHandle::GOperationHandle()
{
	FCreateObjectParams params{};
	params.Owner = this;

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
	// InputComponent->OnMouseWheelDelegate.Bind(this, &GOperationHandle::OnMouseMoved);
	// 绑定键盘鼠标事件
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &GOperationHandle::ExecuteInput);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GOperationHandle::OnMouseMoved);
}

void GOperationHandle::SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer)
{
	XAxisComponent->SetRenderLayerType(mesh_render_layer);
	YAxisComponent->SetRenderLayerType(mesh_render_layer);
	ZAxisComponent->SetRenderLayerType(mesh_render_layer);
}

void GOperationHandle::ResetColor()
{
	ResetColor(XAxisComponent, {1.f, 0.f, 0.f, 1.f});
	ResetColor(YAxisComponent, {0.f, 1.f, 0.f, 1.f});
	ResetColor(ZAxisComponent, {0.f, 0.f, 1.f, 1.f});
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
	
}

void GOperationHandle::ExecuteInput()
{
	if (FInput::IsMouseButtonPressed(VK_LBUTTON))
	{
		// OnMouseMoved();
	}
}

void GOperationHandle::Tick(float DeltaTime)
{
	GActorObject::Tick(DeltaTime);
}

void GOperationHandle::OnMouseMoved(int x, int y)
{
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
	}
	else
	{
		ResetColor();
	}
}
