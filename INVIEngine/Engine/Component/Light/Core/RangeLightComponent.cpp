#include "EngineMinimal.h"

#include "RangeLightComponent.h"

CRangeLightComponent::CRangeLightComponent(): CLightComponent(), StartAttenuation(0), EndAttenuation(0), Kc(0), Kl(0), Kq(0)
{
}

CRangeLightComponent::~CRangeLightComponent()
{
}

void CRangeLightComponent::BeginInit()
{
	CLightComponent::BeginInit();
}

void CRangeLightComponent::Tick(float DeltaTime)
{
	CLightComponent::Tick(DeltaTime);
}

void CRangeLightComponent::SetStartAttenuation(float Start)
{
	StartAttenuation = Start;
}

void CRangeLightComponent::SetEndAttenuation(float end)
{
	EndAttenuation = end;
}

void CRangeLightComponent::SetKc(float c)
{
	Kc = c;
}

void CRangeLightComponent::SetKl(float l)
{
	Kl = l;
}

void CRangeLightComponent::SetKq(float q)
{
	Kq = q;
}
