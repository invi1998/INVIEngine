#pragma once

#include "EngineMinimal.h"
#include "Core/LightComponent.h"

class CPointLightComponent : public CLightComponent
{
public:
	CPointLightComponent();
	~CPointLightComponent() override;

public:
	void SetStartAttenuation(float Start);
	void SetEndAttenuation(float end);
	void SetKc(float c);
	void SetKl(float l);
	void SetKq(float q);

	float GetStartAttenuation() const { return StartAttenuation; }
	float GetEndAttenuation() const { return EndAttenuation; }
	float GetKc() const { return Kc; }
	float GetKl() const { return Kl; }
	float GetKq() const { return Kq; }

protected:
	float StartAttenuation; // 开始衰减
	float EndAttenuation;   // 结束衰减

	float Kc;               // 非线性衰减常数项
	float Kl;               // 非线性衰减一次项
	float Kq;               // 非线性衰减二次项
};

