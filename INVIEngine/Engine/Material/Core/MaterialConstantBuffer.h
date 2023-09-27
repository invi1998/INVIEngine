#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					// 4�ֽ�
	float Roughness; // ���ʴֲڶ�		// 4�ֽ�
	int Reserved1;					// 4�ֽ�
	int Reserved2;					// 4

	XMFLOAT4 BaseColor;				// 16�ֽ�
	XMFLOAT4X4 Transformation;	// 64�ֽ�

};
