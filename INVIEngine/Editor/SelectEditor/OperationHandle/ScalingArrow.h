#pragma once
#include "OperationHandle.h"

class GScalingArrow : public GOperationHandle
{
public:
	GScalingArrow();

	void SetMesh() override;

	void OnMouseMoved(int x, int y) override;

	void OnMouseLeftDown(int x, int y) override;

	void OnMousePressed(int x, int y);

protected:
	float LastT2Value;
};

