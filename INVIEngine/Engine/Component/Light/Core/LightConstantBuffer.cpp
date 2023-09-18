#include "LightConstantBuffer.h"

#include "LightType.h"


FLight::FLight(): LightIntensity(), StartAttenuation(1.0f), LightDirection(), EndAttenuation(10.f), LightPosition(),
                  LightType(ELightType::DirectionalLight)
{
}

FLightConstantBuffer::FLightConstantBuffer()
{
	memset(&SceneLights, 0, sizeof(SceneLights));
}
