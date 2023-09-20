#pragma once
#include "EngineMinimal.h"
#include "Core/LightComponent.h"
#include "Core/RangeLightComponent.h"

class CSpotLightComponent : public CRangeLightComponent
{
public:
	CSpotLightComponent();
	~CSpotLightComponent() override;

	float GetSpotInnerCornerPhi() const;
	float GetSpotOuterCornerTheta() const;

	void SetSpotInnerCornerPhi(float phi);
	void SetSpotOuterCornerTheta(float theta);

private:
	float SpotInnerCornerPhi;		// 指定了聚光半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
	float SpotOuterCornerTheta;		// LightDir向量和SpotDir向量之间的夹角。在聚光内部的话θ值应该比ϕ值小
};

