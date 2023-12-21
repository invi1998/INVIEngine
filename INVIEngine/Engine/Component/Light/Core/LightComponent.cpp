#include "EngineMinimal.h"

#include "LightComponent.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Manage/LightManager.h"

CLightComponent::CLightComponent()
	: LightIntensity(1.f, 1.f, 1.f)
{
	GetLightManger()->AddLight(this);
}

CLightComponent::~CLightComponent()
{
	
}

void CLightComponent::BeginInit()
{
	CTransformationComponent::BeginInit();
}

void CLightComponent::Tick(float DeltaTime)
{
	CTransformationComponent::Tick(DeltaTime);
}

void CLightComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	CTransformationComponent::SetPosition(InNewPosition);

	LightMesh->SetPosition(InNewPosition);
}

void CLightComponent::SetRotation(const fvector_3d& InNewRotation)
{
	CTransformationComponent::SetRotation(InNewRotation);

	LightMesh->SetRotation(InNewRotation * (-1.f));
}

void CLightComponent::SetScale(const fvector_3d& InNewScale)
{
	CTransformationComponent::SetScale(InNewScale);

	LightMesh->SetScale(InNewScale);
}

void CLightComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	CTransformationComponent::SetForwardVector(InForwardVector);

	LightMesh->SetForwardVector(InForwardVector);
}

void CLightComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	CTransformationComponent::SetRightVector(InRightVector);

	LightMesh->SetRightVector(InRightVector);
}

void CLightComponent::SetUpVector(const XMFLOAT3& InUPVector)
{
	CTransformationComponent::SetUpVector(InUPVector);

	LightMesh->SetUpVector(InUPVector);
}

void CLightComponent::SetLightIntensity(const XMFLOAT3& Intensity)
{
	LightIntensity = Intensity;
}

void CLightComponent::SetLightType(ELightType type)
{
	LightType = type;
}

void CLightComponent::SetCastShadow(bool shadow) const
{
	LightMesh->SetCastShadow(shadow);
}

void CLightComponent::SetLightMesh(CMeshComponent* inLightMesh)
{
	LightMesh = inLightMesh;
	SetCastShadow(false);	// 默认灯光模型都不投射阴影
}


