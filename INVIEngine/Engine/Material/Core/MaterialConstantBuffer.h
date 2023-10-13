#pragma once

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					
	float Roughness;			// 材质粗糙度
	int BaseColorIndex;			// 材质贴图索引，默认-1
	int NormalIndex;			// 法线贴图索引

	XMFLOAT4 BaseColor;				// 16字节
	XMFLOAT4X4 Transformation;	// 64字节

};
