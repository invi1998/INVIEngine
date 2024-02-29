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
	float Param0;		// �Զ������
	float Param1;
	float Param2;

	XMFLOAT4 BaseColor;				// 16�ֽ�

	XMFLOAT3 SpecularColor;			// �߹���ͼ��ɫ
	float RefractiveValue;			// ������

	XMFLOAT3 FresnelF0;				// �����F0
	float Transparency;				// ͸����

	XMFLOAT4X4 Transformation;	// 64�ֽ�

	XMFLOAT3 Metallicity;	// 12�ֽ�
	float xx4;

};
