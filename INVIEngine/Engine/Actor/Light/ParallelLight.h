#pragma once

#include "EngineMinimal.h"
#include "Component/Light/ParallelLightComponent.h"
#include "Core/CoreObject/CoreMinimalObject.h"

class GParallelLight : public CCoreMinimalObject
{
	CVARIABLE()
		CParallelLightComponent* ParallelLightComponent;

public:
	GParallelLight();
	~GParallelLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
	virtual void SetPosition(const XMFLOAT3& position);
	virtual void SetRotation(const fvector_3d& rotation);
	virtual void SetScale(const fvector_3d& scale);
};

