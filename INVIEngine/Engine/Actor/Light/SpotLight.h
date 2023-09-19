#pragma once

#include "EngineMinimal.h"

#include "Actor/Core/ActorObject.h"
#include "Core/Light.h"

class CSpotLightComponent;

class GSpotLight : public CLight
{
public:
	GSpotLight();
	~GSpotLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

public:
	void SetStartAttenuation(float Start);
	void SetEndAttenuation(float end);

	float GetStartAttenuation() const;
	float GetEndAttenuation() const;

};

