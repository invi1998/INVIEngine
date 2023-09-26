#pragma once

#include "EngineMinimal.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	XMFLOAT4X4 TextureTransformation;
};

