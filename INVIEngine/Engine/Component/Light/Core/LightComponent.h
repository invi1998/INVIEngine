#pragma once

#include "EngineMinimal.h"
#include "LightType.h"
#include "Component/TransformationComponent.h"
#include "Interface/DirectXDeviceInterface.h"

class CMeshComponent;

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
	CVARIABLE()
		CMeshComponent* LightMesh;

public:
	CLightComponent();
	~CLightComponent() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

public:
	CMeshComponent* GetLightMesh() { return LightMesh; }

	virtual void SetPosition(const XMFLOAT3& InNewPosition) override;
	virtual void SetRotation(const fvector_3d& InNewRotation) override;
	virtual void SetScale(const fvector_3d& InNewScale) override;

	virtual void SetForwardVector(const XMFLOAT3& InForwardVector) override;
	virtual void SetRightVector(const XMFLOAT3& InRightVector) override;
	virtual void SetUpVector(const XMFLOAT3& InUPVector) override;

	virtual void SetLightIntensity(XMFLOAT3& Intensity);
	virtual void SetLightType(ELightType type);
	virtual void SetStartAttenuation(float Start);
	virtual void SetEndAttenuation(float end);

	virtual const XMFLOAT3& GetLightIntensity() const { return LightIntensity; }
	virtual ELightType GetLightType() const { return LightType; }
	virtual float GetStartAttenuation() const { return StartAttenuation; }
	virtual float GetEndAttenuation() const { return EndAttenuation; }

protected:
	void SetLightMesh(CMeshComponent* inLightMesh);


protected:
	XMFLOAT3 LightIntensity;
	ELightType LightType;
	float StartAttenuation; // ¿ªÊ¼Ë¥¼õ
	float EndAttenuation;   // ½áÊøË¥¼õ
};

