#include "EngineMinimal.h"

#include "ActorObject.h"

#include "Component/TransformationComponent.h"


GActorObject::GActorObject()
{
	TransformationComponent = CreateObject<CTransformationComponent>(this, new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
	TransformationComponent->SetPosition(InNewPosition);
}

void GActorObject::SetPosition(const XMVECTOR& InNewPosition)
{
	XMFLOAT3 pos{};
	XMStoreFloat3(&pos, InNewPosition);
	SetPosition(pos);
}

void GActorObject::SetRotation(const fvector_3d& InRotation)
{
	TransformationComponent->SetRotation(InRotation);
}

void GActorObject::SetScale(const fvector_3d& InNewScale)
{
	TransformationComponent->SetScale(InNewScale);
}

void GActorObject::SetForwardVector(const XMFLOAT3& InForward)
{
	TransformationComponent->SetForwardVector(InForward);
}

void GActorObject::SetForwardVector(const XMVECTOR& InForward)
{
	XMFLOAT3 fw{};
	XMStoreFloat3(&fw, InForward);
	SetForwardVector(fw);
}

void GActorObject::SetRightVector(const XMVECTOR& InRight)
{
	XMFLOAT3 right{};
	XMStoreFloat3(&right, InRight);
	SetRightVector(right);
}

void GActorObject::SetUpVector(const XMVECTOR& InUp)
{
	XMFLOAT3 up{};
	XMStoreFloat3(&up, InUp);
	SetUpVector(up);
}

void GActorObject::SetRightVector(const XMFLOAT3& InRight)
{
	TransformationComponent->SetRightVector(InRight);
}

void GActorObject::SetUpVector(const XMFLOAT3& InUp)
{
	TransformationComponent->SetUpVector(InUp);
}

XMFLOAT3& GActorObject::GetPosition()
{
	return TransformationComponent->GetPosition();
}

XMFLOAT3& GActorObject::GetRotation()
{
	return TransformationComponent->GetRotation();
}

XMFLOAT3& GActorObject::GetScale()
{
	return TransformationComponent->GetScale();
}

XMFLOAT3& GActorObject::GetForwardVector()
{
	return TransformationComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector()
{
	return TransformationComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUpVector()
{
	return TransformationComponent->GetUpVector();
}
