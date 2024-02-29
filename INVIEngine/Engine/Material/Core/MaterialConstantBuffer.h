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
	float Param0;		// 自定义参数
	float Param1;
	float Param2;

	XMFLOAT4 BaseColor;				// 16字节

	XMFLOAT3 SpecularColor;			// 高光贴图颜色
	float RefractiveValue;			// 折射率

	XMFLOAT3 FresnelF0;				// 菲尼尔F0
	float Transparency;				// 透明度

	XMFLOAT4X4 Transformation;	// 64字节

	XMFLOAT3 Metallicity;	// 12字节
	float xx4;

};
