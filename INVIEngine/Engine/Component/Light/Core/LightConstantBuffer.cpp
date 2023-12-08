#include "EngineMinimal.h"

#include "LightConstantBuffer.h"

#include "LightType.h"


FLight::FLight(): LightIntensity(), StartAttenuation(1.0f), LightDirection(), EndAttenuation(10.f), LightPosition(),
                  LightType(ELightType::DirectionalLight), Kc(1.0f), Kl(0.7f), Kq(1.8f), SpotInnerCornerPhi(30.f),
                  SpotOuterCornerTheta(20.f),
                  xxs1(0), xxs2(0), xxs3(0), ShadowTransform(EngineMath::IdentityMatrix4x4())
{
}

FLightConstantBuffer::FLightConstantBuffer()
{
	memset(&SceneLights, 0, sizeof(SceneLights));
}
