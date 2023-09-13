#pragma once
#include "Core/LightComponent.h"

class CMeshComponent;
class CCustomMeshComponent;

// Æ½ÐÐ¹â
class CParallelLightComponent : public CLightComponent
{
	CVARIABLE()
	CMeshComponent* ParallelLightMesh;

public:
	CParallelLightComponent();
	~CParallelLightComponent() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;
};

