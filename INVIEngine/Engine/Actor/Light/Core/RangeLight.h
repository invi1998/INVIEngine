#pragma once

#include "EngineMinimal.h"
#include "Light.h"

class GRangeLight : public CLight
{
public:
	GRangeLight();
	~GRangeLight() override;

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

