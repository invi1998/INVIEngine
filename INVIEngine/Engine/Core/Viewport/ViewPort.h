#pragma once

class FViewPort
{
public:
	FViewPort();

	void ViewPortInit();

	void ResetViewPort(UINT width, UINT height);
	void ResetViewPortRect(UINT width, UINT height);

	// ����Ļ�ӿ����
	D3D12_VIEWPORT ViewPortInfo;		// �ӿ���Ϣ
	D3D12_RECT ViewPortRect;			// �ӿڲü�����

	// ���������λ�ã�up, right, look �ӿھ���
	XMFLOAT4X4 ViewMatrix;

	// ���ӷ�Χ ���ӽǶ� ͶӰ����
	XMFLOAT4X4 ProjectionMatrix;

};

