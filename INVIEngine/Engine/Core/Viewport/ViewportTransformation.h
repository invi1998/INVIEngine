#pragma once

#include "EngineMinimal.h"

class FViewportTransformation
{
public:
	FViewportTransformation();

	XMFLOAT4 CameraPosition;
	XMFLOAT4X4 ViewProjectionMatrix;
};

