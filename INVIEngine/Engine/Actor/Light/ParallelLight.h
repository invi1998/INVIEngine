#pragma once

#include "EngineMinimal.h"
#include "Component/Light/ParallelLightComponent.h"
#include "Core/Light.h"

class GParallelLight : public CLight
{
public:
	GParallelLight();
	~GParallelLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

