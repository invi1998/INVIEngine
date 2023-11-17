#include "EngineMinimal.h"

#include "ActorObject.h"

#include "Component/TransformationComponent.h"


GActorObject::GActorObject()
{
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
	TransformationComponent->SetPosition(InNewPosition);
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
