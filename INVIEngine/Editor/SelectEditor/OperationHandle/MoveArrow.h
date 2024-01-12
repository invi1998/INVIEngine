#pragma once
#include "OperationHandle.h"

// Æ½ÒÆ¼ýÍ·
class GMoveArrow : public GOperationHandle
{
public:
	GMoveArrow();

	void SetMesh() override;

	void OnMouseMoved(int x, int y) override;

	void OnMouseLeftDown(int x, int y) override;

	void OnMousePressed(int x, int y);
};

