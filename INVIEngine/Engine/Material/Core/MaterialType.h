#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
	Lambert = 0,	// �����ز���
	HalfLambert,	// �������ز���
	Phong,			// phong����
	BlinnPhong,		// BlinnPhong����
	WrapLight,		// WrapLight���ʣ�����ģ��Ƥ��ʱʹ�õĲ���

	Fresnel = 100,	// ������

	Max,			// Ĭ�ϲ���
};
