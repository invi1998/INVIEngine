#pragma once

enum EMaterialType : UINT;

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;					
	float Roughness;			// ���ʴֲڶ�
	int BaseColorIndex;			// ������ͼ������Ĭ��-1
	int NormalIndex;			// ������ͼ����

	int SpecularIndex;			// �߹���ͼ����
	int XX1;
	int XX2;
	int XX3;

	XMFLOAT4 BaseColor;				// 16�ֽ�

	XMFLOAT3 SpecularColor;			// �߹���ͼ��ɫ
	int XX6;

	XMFLOAT3 FresnelF0;				// �����F0
	float xx7;

	XMFLOAT4X4 Transformation;	// 64�ֽ�

};
