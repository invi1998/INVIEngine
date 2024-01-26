#pragma once

#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();
	~CTransformationComponent() override;

	virtual void SetPosition(const XMFLOAT3& InPosition);
	virtual void SetRotation(const XMFLOAT3& InRotation);
	virtual void SetScale(const XMFLOAT3& InScale);

	virtual void SetForwardVector(const XMFLOAT3& InForward);
	virtual void SetRightVector(const XMFLOAT3& InRight);
	virtual void SetUpVector(const XMFLOAT3& InUp);

	XMFLOAT3& GetPosition() { return Position; }
	XMFLOAT3& GetRotation();
	XMFLOAT3& GetScale() { return Scale; }
	XMFLOAT3& GetForwardVector()
	{
		return ForwardVector;
	}
	XMFLOAT3 &GetRightVector()
	{
		return RightVector;
	}
	XMFLOAT3 &GetUpVector()
	{
		return UpVector;
	}

public:
	// У�� Up, Right, Look(Forward)
	void CorrectionVector();

	fvector_3d GetCorrectionPosition();
	void GetCorrectionPosition(fvector_3d& InPos3);

	XMFLOAT4X4 CalculateViewMatrix();

private:

	XMFLOAT3 Position;			// λ����Ϣ
	XMFLOAT3 Rotation;			// ��ת��Ϣ
	XMFLOAT3 Scale;				// ������Ϣ

	XMFLOAT3 ForwardVector;		// forward	���������� ��ͬ���������look����
	XMFLOAT3 RightVector;		// right	�ҷ�������
	XMFLOAT3 UpVector;			// up		�Ϸ�������
};

