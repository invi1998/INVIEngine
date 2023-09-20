#include "RangeLight.h"

#include "Component/Light/Core/RangeLightComponent.h"

GRangeLight::GRangeLight()
{
}

GRangeLight::~GRangeLight()
{
}

void GRangeLight::SetStartAttenuation(float Start)
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->SetStartAttenuation(Start);
	}
}

void GRangeLight::SetEndAttenuation(float end)
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->SetEndAttenuation(end);
	}
}

void GRangeLight::SetKc(float c)
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->SetKc(c);
	}
}

void GRangeLight::SetKl(float l)
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->SetKl(l);
	}
}

void GRangeLight::SetKq(float q)
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->SetKq(q);
	}
}

float GRangeLight::GetStartAttenuation() const
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->GetStartAttenuation();
	}
	return 0.f;
}

float GRangeLight::GetEndAttenuation() const
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->GetEndAttenuation();
	}
	return 0.f;
}

float GRangeLight::GetKc() const
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->GetKc();
	}
	return 0.f;
}

float GRangeLight::GetKl() const
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->GetKl();
	}
	return 0.f;
}

float GRangeLight::GetKq() const
{
	if (const auto RangeLightComponent = dynamic_cast<CRangeLightComponent*>(GetLightComponent()))
	{
		return RangeLightComponent->GetKq();
	}
	return 0.f;
}
