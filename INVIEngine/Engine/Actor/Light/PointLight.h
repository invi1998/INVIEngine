#pragma once

#include "EngineMinimal.h"

#include "Core/RangeLight.h"

class GPointLight : public GRangeLight
{
public:
	GPointLight();
	~GPointLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

};

