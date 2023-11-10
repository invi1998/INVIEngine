#pragma once

#include "EngineMinimal.h"

// 渲染管线状态类型 枚举

enum EPipelineState : UINT
{
	Transparent = 0,
	AlphaTest,
	Solid	= 4,				// 实体
	Wireframe	= 5,			// 线框
};

