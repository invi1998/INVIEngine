#pragma once

enum EMeshRenderLayerType : int
{
	RENDER_LAYER_OPAQUE = 0,			// 不透明层
	RENDER_LAYER_TRANSPARENT = 1,		// 透明层
	RENDER_LAYER_ALPHA_TEST = 2,		// alpha测试层
	RENDER_LAYER_BACKGROUND = 3,		// 背景层
	RENDER_LAYER_OPAQUE_REFLECT = 4,	// 不透明反射层
	RENDER_LAYER_OPAQUE_SHADOW = 8,		// 不透明阴影层
	RENDER_LAYER_SELECT = 9,			// 选择层
	RENDER_LAYER_OPERATE = 10,			// 操作层
	RENDER_LAYER_ROT_PLANE = 11,		// 旋转平面层


	RENDER_LAYER_SCREEN_NORMAL = 20,	// 屏幕法线层
};


