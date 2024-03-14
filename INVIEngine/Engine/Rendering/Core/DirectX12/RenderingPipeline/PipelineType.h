#pragma once

#include "EngineMinimal.h"

// 渲染管线状态类型 枚举（PSO）
enum EPipelineState : UINT
{
	Transparent = 0,				// 透明
	AlphaTest = 1,					// Alpha测试
	Background = 2,					// 背景层
	Reflector = 3,					// 反射层
	Solid = 4,						// 实体
	Wireframe = 5,					// 线框
	OrthographicShadow = 6,			// 正交阴影
	PerspectiveShadow = 7,			// 透视阴影
	ViewtianeShadow = 8,			// 全向阴影
	Selection = 9,					// 选择
	Operation = 10,					// 操作手柄
	RotatePlane = 11,				// 旋转平面


	ScreenNormal = 20,				// 屏幕法线
	SSA0 = 21,						// SSAO
};


// 渲染条件枚举类型
enum ERenderCondition : UINT
{
	RC_Always = 0,		// 总是绘制
	RC_Shadow = 1,		// 绘制阴影需要进行判断
};
