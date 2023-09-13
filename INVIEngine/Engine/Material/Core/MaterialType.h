#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
	// �ֲ�����
	Lambert = 0,					// �����ز���	ֽ��
	HalfLambert,					// �������ز��� �ֳ�����
	Phong,							// phong����	����
	BlinnPhong,						// BlinnPhong���� ����
	WrapLight,						// WrapLight���ʣ�����ģ��Ƥ��ʱʹ�õĲ���
	Minnaert,						// �������,����޵Ȳ��ϲ���
	Banded,							// ������ͨЧ��
	GradualBanded,					// ������Ŀ�ͨЧ��
	CustomBanded,					// �Զ��忨ͨЧ��
	Back,							// ��ʯ����
	AnisotoropyKaijiyakay,			// GDC 2004�� KajiyaKay ͷ����Ⱦģ�ͣ�����������Ⱦ��
	OrenNayar,						// GDC �ֲڱ���

	// ������ʾ
	BaseColor,						// ������ʾ������ɫ
	Normal,							// ������ʾ���㷨�ߣ��ֲ����ߣ�
	WorldNormal,					// ������ʾ���編��

	PBR = 20,						// PBR ������ʵ�������Ⱦ

	Fresnel = 100,					// ������

	Max,							// Ĭ�ϲ���
};



// ��ʾ״̬���㣬�߿� �棩
enum EMaterialDisplayStatusType : UINT
{
	DefaultDisplay,			// Ĭ����ʾ
	PointDisplay,			// ��ʾ��
	WireframeDisplay,		// �߿���ʾ
	TriangleDisplay = 4,	// ����������ʾ
};

