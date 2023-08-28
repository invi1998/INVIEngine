#pragma once

#include "EngineMinimal.h"
#include "Mesh/Core/ObjectTransformation.h"

class FMaterialConstantBuffer
{
	void Update(int i, FObjectTransformation* object_transformation);
};

