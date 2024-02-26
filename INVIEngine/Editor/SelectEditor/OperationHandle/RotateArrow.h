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
	float LastT2Value = 0.0f;	// 上一次的T2值
};


// 四元数旋转操作对象
// 实现思路：
// 1: 获取物体的旋转
// 2: 将物体的旋转转换为四元数
// 3：根据鼠标拖拽的轴的方向向量，计算出旋转的四元数（四元数的旋转轴和旋转角度）
// 4：将物体的旋转四元数和计算出的旋转四元数相乘，得到新的旋转四元数
// 5：将新的旋转四元数转换为欧拉角，设置物体的旋转
