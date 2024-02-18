#pragma once
#include "OperationHandle.h"

// 平移箭头
class GMoveArrow : public GOperationHandle
{
public:
	GMoveArrow();

	void SetMesh() override;

	void OnMouseMoved(int x, int y) override;

	void OnMouseLeftDown(int x, int y) override;

	void OnMouseLeftUp(int x, int y) override;

	void ExecuteInput() override;

	void OnMousePressed(int x, int y);

protected:
	XMVECTOR GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation) override;

protected:
	XMVECTOR RelativePosition{};	// 相对于选中物体的位置
};

