#pragma once

#include "EngineMinimal.h"

#include "Actor/Core/ActorObject.h"

class CSpotLightComponent;

class GSpotLight : public GActorObject
{
	CVARIABLE()
		CSpotLightComponent* SpotLightComponent;

public:
	GSpotLight();
	~GSpotLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
	virtual void SetPosition(const XMFLOAT3& position) override;
	virtual void SetRotation(const fvector_3d& rotation) override;
	virtual void SetScale(const fvector_3d& scale) override;

};

