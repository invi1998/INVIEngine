#pragma once

#include "EngineMinimal.h"

// 渲染管线状态类型 枚举

enum EPipelineState : UINT
{
	Transparent = 0,
	AlphaTest = 1,
	Background = 2,					// 背景层
	Reflector = 3,					// 反射层
	Solid	= 4,				// 实体
	Wireframe	= 5,			// 线框
	Shadow = 6,					// 阴影
};


// 渲染条件枚举类型
enum ERenderCondition : UINT
{
	RC_Always = 0,		// 总是绘制
	RC_Shadow = 1,		// 绘制阴影
};
