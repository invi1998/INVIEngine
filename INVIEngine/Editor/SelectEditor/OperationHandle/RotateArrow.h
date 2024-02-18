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

protected:
	void GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation, XMVECTOR& OutDirection) override;

protected:
	float LastT2Value = 0.0f;	// 上一次的T2值
};

