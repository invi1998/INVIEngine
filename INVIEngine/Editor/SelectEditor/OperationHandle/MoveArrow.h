#pragma once
#include "OperationHandle.h"

// ƽ�Ƽ�ͷ
class GMoveArrow : public GOperationHandle
{
public:
	GMoveArrow();

	void SetMesh() override;

	void OnMouseMoved(int x, int y) override;

	void OnMouseLeftDown(int x, int y) override;

	void OnMousePressed(int x, int y);

protected:
	float GetMouseMoveDistance(int x, int y, XMVECTOR& ActorLocation, XMVECTOR& DragDirection);

protected:
	XMVECTOR RelativePosition{};	// �����ѡ�������λ��
};

