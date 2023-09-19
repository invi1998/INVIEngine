#pragma once

#include "EngineMinimal.h"
#include "Core/LightComponent.h"

class CSpotLightComponent : public CLightComponent
{
public:
	CSpotLightComponent();
	~CSpotLightComponent() override;

public:
	void SetStartAttenuation(float Start);
	void SetEndAttenuation(float end);

	float GetStartAttenuation() const { return StartAttenuation; }
	float GetEndAttenuation() const { return EndAttenuation; }

protected:
	float StartAttenuation; // ��ʼ˥��
	float EndAttenuation;   // ����˥��
};

