#pragma once

#include "Core/RangeLight.h"

class GSpotLight : public GRangeLight
{
public:
	GSpotLight();
	~GSpotLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

	float GetConicalInnerCorner() const;
	float GetConicalOuterCorner() const;

	void SetConicalInnerCorner(float InConicalInnerCorner);
	void SetConicalOuterCorner(float InConicalOuterCorner);
};

