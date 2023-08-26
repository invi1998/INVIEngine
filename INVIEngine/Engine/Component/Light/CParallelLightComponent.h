#pragma once
#include "Core/LightComponent.h"

// Æ½ÐÐ¹â
class CParallelLightComponent : public CLightComponent
{
public:
	CParallelLightComponent();
	~CParallelLightComponent() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

