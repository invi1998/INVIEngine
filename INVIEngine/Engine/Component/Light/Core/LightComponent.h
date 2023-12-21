#pragma once

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
	CMeshComponent* GetLightMesh() const { return LightMesh; }

	virtual void SetPosition(const XMFLOAT3& InNewPosition) override;
	virtual void SetRotation(const fvector_3d& InNewRotation) override;
	virtual void SetScale(const fvector_3d& InNewScale) override;

	virtual void SetForwardVector(const XMFLOAT3& InForwardVector) override;
	virtual void SetRightVector(const XMFLOAT3& InRightVector) override;
	virtual void SetUpVector(const XMFLOAT3& InUPVector) override;

	virtual void SetLightIntensity(const XMFLOAT3& Intensity);
	virtual void SetLightType(ELightType type);

	virtual const XMFLOAT3& GetLightIntensity() const { return LightIntensity; }
	virtual ELightType GetLightType() const { return LightType; }
	void SetCastShadow(bool shadow) const;


protected:
	void SetLightMesh(CMeshComponent* inLightMesh);


protected:
	XMFLOAT3 LightIntensity;
	ELightType LightType;
};

