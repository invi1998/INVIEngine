#pragma once

class FViewPort
{
public:
	FViewPort();

	void ViewPortInit();

	// ���������λ�ã�up, right, look �ӿھ���
	XMFLOAT4X4 ViewMatrix;

	// ���ӷ�Χ ���ӽǶ� ͶӰ����
	XMFLOAT4X4 ProjectionMatrix;

};

