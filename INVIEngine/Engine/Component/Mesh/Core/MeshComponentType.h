#pragma once

enum EMeshRenderLayerType : int
{
	RENDER_LAYER_OPAQUE = 0,			// ��͸����
	RENDER_LAYER_TRANSPARENT = 1,		// ͸����
	RENDER_LAYER_ALPHA_TEST = 2,		// alpha���Բ�
	RENDER_LAYER_BACKGROUND = 3,		// ������
	RENDER_LAYER_OPAQUE_REFLECT = 4,	// ��͸�������
	RENDER_LAYER_OPAQUE_SHADOW = 8,		// ��͸����Ӱ��
	RENDER_LAYER_SELECT = 9,			// ѡ���
	RENDER_LAYER_OPERATE = 10,			// ������
	RENDER_LAYER_ROT_PLANE = 11,		// ��תƽ���


	RENDER_LAYER_SCREEN_NORMAL = 20,	// ��Ļ���߲�
};


