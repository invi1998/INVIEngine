#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
	Lambert = 0,	// 兰伯特材质
	HalfLambert,	// 半兰伯特材质
	Phong,			// phong材质
	BlinnPhong,		// BlinnPhong材质

	Fresnel = 100,	// 菲涅尔

	Max,			// 默认材质
};
