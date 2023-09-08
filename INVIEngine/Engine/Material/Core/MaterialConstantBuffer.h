#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	EMaterialType MaterialType;		// 4�ֽ�
	int Reserved1;					// 4�ֽ�
	int Reserved2;					// 4
	int Reserved3;					// 4

	XMFLOAT4 BaseColor;				// 16�ֽ�
	float Roughness; // ���ʴֲڶ�	// 4�ֽ�

	XMFLOAT4X4 MaterialProjectionMatrix;	// 64�ֽ�
};
