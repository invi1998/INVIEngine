#pragma once
#include "Core/LightComponent.h"

// ƽ�й�
class CParallelLightComponent : public CLightComponent
{
public:
	CParallelLightComponent();
	~CParallelLightComponent() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

