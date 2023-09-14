#pragma once

#include "EngineMinimal.h"
#include "Actor/Core/ActorObject.h"
#include "Component/Light/ParallelLightComponent.h"
#include "Core/CoreObject/CoreMinimalObject.h"

class GParallelLight : public GActorObject
{
	CVARIABLE()
		CParallelLightComponent* ParallelLightComponent;

public:
	GParallelLight();
	~GParallelLight() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
	virtual void SetPosition(const XMFLOAT3& position) override;
	virtual void SetRotation(const fvector_3d& rotation) override;
	virtual void SetScale(const fvector_3d& scale) override;
};

