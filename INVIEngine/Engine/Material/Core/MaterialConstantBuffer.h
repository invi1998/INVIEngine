#pragma once

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					
	float Roughness;			// 材质粗糙度
	int BaseColorIndex;			// 材质贴图索引，默认-1
	int NormalIndex;			// 法线贴图索引

	int SpecularIndex;			// 高光贴图索引
	int XX1;
	int XX2;
	int XX3;

	XMFLOAT4 BaseColor;				// 16字节

	XMFLOAT3 SpecularColor;			// 高光贴图颜色
	int XX6;

	XMFLOAT3 FresnelF0;				// 菲尼尔F0
	float xx7;

	XMFLOAT4X4 Transformation;	// 64字节

};
