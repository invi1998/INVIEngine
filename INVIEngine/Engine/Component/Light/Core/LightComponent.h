#pragma once

#include "EngineMinimal.h"
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

	void SetLightIntensity(XMFLOAT3& Intensity);

	const XMFLOAT3& GetLightIntensity() const { return LightIntensity; }

protected:
	void SetLightMesh(CMeshComponent* inLightMesh);


protected:
	XMFLOAT3 LightIntensity;

};

