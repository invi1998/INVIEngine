#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	EMaterialType MaterialType;
	XMFLOAT4 BaseColor;
	float Roughness; // ²ÄÖÊ´Ö²Ú¶È

	XMFLOAT4X4 MaterialProjectionMatrix;
};
