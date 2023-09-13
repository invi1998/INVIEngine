#pragma once

#include "EngineMinimal.h"
#include "Component/Light/ParallelLightComponent.h"
#include "Core/CoreObject/CoreMinimalObject.h"

class CParallelLight : public CCoreMinimalObject
{
	CVARIABLE()
		CParallelLightComponent* ParallelLightComponent;

public:
	CParallelLight();
	~CParallelLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

