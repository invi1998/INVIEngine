#pragma once

#include "EngineMinimal.h"
#include "Component/TransformationComponent.h"

class CMaterial;

class CMeshComponent : public CTransformationComponent
{
	CVARIABLE()
		std::vector<CMaterial*> Materials;

public:
	CMeshComponent();

	UINT GetMaterialNum() const;

	std::vector<CMaterial*>* GetMaterial() { return &Materials; }

};

