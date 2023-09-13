#pragma once

#include "EngineMinimal.h"
#include "Component/TransformationComponent.h"
#include "Interface/DirectXDeviceInterface.h"

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
public:
	CLightComponent();
	~CLightComponent() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

