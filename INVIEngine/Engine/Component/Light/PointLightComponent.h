#pragma once

#include "EngineMinimal.h"
#include "Core/LightComponent.h"
#include "Core/RangeLightComponent.h"

class CPointLightComponent : public CRangeLightComponent
{
public:
	CPointLightComponent();
	~CPointLightComponent() override;
};

