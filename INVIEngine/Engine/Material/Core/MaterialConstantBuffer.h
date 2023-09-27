#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					// 4字节
	float Roughness; // 材质粗糙度		// 4字节
	int Reserved1;					// 4字节
	int Reserved2;					// 4

	XMFLOAT4 BaseColor;				// 16字节
	XMFLOAT4X4 Transformation;	// 64字节

};
