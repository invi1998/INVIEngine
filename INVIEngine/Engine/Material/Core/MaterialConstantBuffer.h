#pragma once

#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/ObjectTransformation.h"

struct FMaterialConstantBuffer : public IDirectXDeviceInterface_Struct
{
	void Update(int i, FObjectTransformation* object_transformation);
};

