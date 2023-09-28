#pragma once

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					
	float Roughness;			// ���ʴֲڶ�
	int BaseColorIndex;	// ������ͼ������Ĭ��-1
	int Reserved2;					// 4

	XMFLOAT4 BaseColor;				// 16�ֽ�
	XMFLOAT4X4 Transformation;	// 64�ֽ�

};
