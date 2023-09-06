#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	XMFLOAT4 BaseColor;
	float Roughness;	// ²ÄÖÊ´Ö²Ú¶È
	EMaterialType MaterialType;

	XMFLOAT4X4 World;

};

