#pragma once

#include "EngineMinimal.h"

enum EMaterialType : UINT
{
	// 局部光照
	Lambert = 0,					// 兰伯特材质	纸张
	HalfLambert,					// 半兰伯特材质 粗超表面
	Phong,							// phong材质	金属
	BlinnPhong,						// BlinnPhong材质 金属
	WrapLight,						// WrapLight材质，早期模拟皮肤时使用的材质
	Minnaert,						// 月球材质,天鹅绒等布料材质
	Banded,							// 基础卡通效果
	GradualBanded,					// 带渐变的卡通效果
	CustomBanded,					// 自定义卡通效果
	Back,							// 玉石材质
	AnisotoropyKaijiyakay,			// GDC 2004年 KajiyaKay 头发渲染模型（各项异性渲染）
	OrenNayar,						// GDC 粗糙表面

	// 单独显示
	BaseColor,						// 单独显示顶点颜色
	Normal,							// 单独显示顶点法线（局部法线）
	WorldNormal,					// 单独显示世界法线

	PBR = 20,						// PBR 基于真实物理的渲染

	Fresnel = 100,					// 菲涅尔

	Max,							// 默认材质
};



// 显示状态（点，线框， 面）
enum EMaterialDisplayStatusType : UINT
{
	DefaultDisplay,			// 默认显示
	PointDisplay,			// 显示点
	WireframeDisplay,		// 线框显示
	TriangleDisplay = 4,	// 三角形面显示
};

