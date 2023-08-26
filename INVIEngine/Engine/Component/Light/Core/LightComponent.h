#pragma once

#include "EngineMinimal.h"
#include "Component/TransformationComponent.h"

class CLightComponent : public CTransformationComponent
{
public:
	CLightComponent();
	~CLightComponent() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

