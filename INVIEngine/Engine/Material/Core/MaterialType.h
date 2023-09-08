#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
	Lambert = 0,	// 兰伯特材质	纸张
	HalfLambert,	// 半兰伯特材质 粗超表面
	Phong,			// phong材质	金属
	BlinnPhong,		// BlinnPhong材质 金属
	WrapLight,		// WrapLight材质，早期模拟皮肤时使用的材质
	Minnaert,		// 月球材质,天鹅绒等布料材质
	Banded,			// 基础卡通效果
	GradualBanded,	// 带渐变的卡通效果
	CustomBanded,	// 自定义卡通效果

	Fresnel = 100,	// 菲涅尔

	Max,			// 默认材质
};
