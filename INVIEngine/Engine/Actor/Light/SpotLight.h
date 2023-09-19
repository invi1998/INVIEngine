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
	void SetKc(float c);
	void SetKl(float l);
	void SetKq(float q);

	float GetStartAttenuation() const;
	float GetEndAttenuation() const;
	float GetKc() const;
	float GetKl() const;
	float GetKq() const;

};

