#pragma once

#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();
	~CTransformationComponent() override;

	void SetPosition(const XMFLOAT3& InPosition);
	void SetForwardVector(const XMFLOAT3& InForward);
	void SetRightVector(const XMFLOAT3& InRight);
	void SetUpVector(const XMFLOAT3& InUp);

	XMFLOAT3 &GetPosition() { return Position; }
	XMFLOAT3 &GetForwardVector() { return ForwardVector; }
	XMFLOAT3 &GetRightVector() { return RightVector; }
	XMFLOAT3 &GetUpVector() { return UpVector; }

public:
	// 校正 Up, Right, Look(Forward)
	void CorrectionVector();

	fvector_3d GetCorrectionPosition();
	void GetCorrectionPosition(fvector_3d& InPos3);

	XMFLOAT4X4 CalculateViewMatrix();

private:

	XMFLOAT3 Position;			// 位置信息

	XMFLOAT3 ForwardVector;		// forward	正方向向量 等同于龙书里的look向量
	XMFLOAT3 RightVector;		// right	右方向向量
	XMFLOAT3 UpVector;			// up		上方向向量
};

