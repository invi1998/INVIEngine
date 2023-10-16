#pragma once

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					
	float Roughness;			// ���ʴֲڶ�
	int BaseColorIndex;			// ������ͼ������Ĭ��-1
	int NormalIndex;			// ������ͼ����

	/*int SpecularIndex;
	int XX1;
	int XX2;
	int XX3;*/

	XMFLOAT4 BaseColor;				// 16�ֽ�

	/*XMFLOAT3 SpecularColor;
	int XX6;*/

	XMFLOAT4X4 Transformation;	// 64�ֽ�

};
