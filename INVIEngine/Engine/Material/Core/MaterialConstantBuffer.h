#pragma once

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					
	float Roughness;			// 材质粗糙度
	int BaseColorIndex;	// 材质贴图索引，默认-1
	int Reserved2;					// 4

	XMFLOAT4 BaseColor;				// 16字节
	XMFLOAT4X4 Transformation;	// 64字节

};
