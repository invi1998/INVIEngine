#pragma once

#include "EngineMinimal.h"

class FViewportTransformation
{
public:
	FViewportTransformation();

	XMFLOAT4X4 ViewProjectionMatrix;
};

