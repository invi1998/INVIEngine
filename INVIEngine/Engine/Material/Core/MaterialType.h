#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
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

	Fresnel = 100,					// ������

	Max,							// Ĭ�ϲ���
};
