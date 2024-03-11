#pragma once

#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();
	~CTransformationComponent() override;

	virtual void SetPosition(const XMFLOAT3& InPosition);
	virtual void SetRotation(const XMFLOAT3& InRotation);
	virtual void SetRotation(const frotator& InRotation);	// ������ת��Ϣ��ʹ��frotator
	virtual void SetScale(const XMFLOAT3& InScale);

	virtual void SetForwardVector(const XMFLOAT3& InForward);
	virtual void SetRightVector(const XMFLOAT3& InRight);
	virtual void SetUpVector(const XMFLOAT3& InUp);

	XMFLOAT3& GetRotation();

	XMVECTOR GetRotationQuat() const;	// ��ȡ��ת��Ԫ��
	void SetRotationQuat(const XMVECTOR& InQuat);	// ������ת��Ԫ��

	FORCEINLINE XMFLOAT3& GetPosition() { return Position; }
	FORCEINLINE XMFLOAT3& GetScale() { return Scale; }
	FORCEINLINE XMFLOAT3& GetForwardVector() { return ForwardVector; }
	FORCEINLINE XMFLOAT3& GetRightVector() { return RightVector; }
	FORCEINLINE XMFLOAT3& GetUpVector() { return UpVector; }

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

