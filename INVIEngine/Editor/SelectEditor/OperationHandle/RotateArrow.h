#pragma once
#include "OperationHandle.h"

class GRotateArrow : public GOperationHandle
{
public:
	GRotateArrow();

	void SetMesh() override;

	void OnMouseMoved(int x, int y) override;

	void OnMouseLeftDown(int x, int y) override;

	void OnMouseLeftUp(int x, int y) override;

	void ExecuteInput() override;

	void OnMousePressed(int x, int y);

	void SetScale(const XMFLOAT3& InNewScale) override;

protected:
	XMVECTOR GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation) override;

protected:
	float LastT2Value = 0.0f;	// ��һ�ε�T2ֵ
};


// ��Ԫ����ת��������
// ʵ��˼·��
// 1: ��ȡ�������ת
// 2: ���������תת��Ϊ��Ԫ��
// 3�����������ק����ķ����������������ת����Ԫ������Ԫ������ת�����ת�Ƕȣ�
// 4�����������ת��Ԫ���ͼ��������ת��Ԫ����ˣ��õ��µ���ת��Ԫ��
// 5�����µ���ת��Ԫ��ת��Ϊŷ���ǣ������������ת
