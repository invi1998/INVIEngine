#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
	Lambert = 0,	// �����ز���
	HalfLambert,	// �������ز���
	Phong,			// phong����
	BlinnPhong,		// BlinnPhong����

	Max,			// Ĭ�ϲ���
};
