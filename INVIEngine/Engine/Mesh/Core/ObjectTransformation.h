#pragma once

#include "EngineMinimal.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	static XMFLOAT4X4 IdentityMatrix4X4();	// ��λ��4x4����
};

