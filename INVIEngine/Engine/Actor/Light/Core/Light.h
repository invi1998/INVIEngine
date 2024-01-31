#pragma once

#include "EngineMinimal.h"
#include "Actor/Core/ActorObject.h"
#include "Component/Light/Core/LightType.h"

class CLightComponent;

class CLight : public GActorObject
{

	CVARIABLE()
		CLightComponent* LightComponent;

public:
	CLight();
	void BeginInit() override;
	~CLight() override;

	void Tick(float DeltaTime) override;

	[[nodiscard]] auto GetLightComponent() const -> CLightComponent*;

public:
	void SetPosition(const XMFLOAT3& InNewPosition) override;
	void SetRotation(const XMFLOAT3& InRotation) override;
	void SetScale(const XMFLOAT3& InNewScale) override;

	virtual void SetCastShadow(bool shadow) const;
	virtual void SetLightIntensity(const XMFLOAT3& Intensity);

protected:
	virtual void SetLightComponent(CLightComponent* newComponent);
};

