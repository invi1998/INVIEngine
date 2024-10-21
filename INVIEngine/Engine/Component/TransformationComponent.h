#pragma once

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();
	~CTransformationComponent() override;

	virtual void SetPosition(const XMFLOAT3& InPosition);
	virtual void SetRotation(const XMFLOAT3& InRotation);
	virtual void SetRotation(const frotator& InRotation);	// 设置旋转信息，使用frotator
	virtual void SetScale(const XMFLOAT3& InScale);

	virtual void SetForwardVector(const XMFLOAT3& InForward);
	virtual void SetRightVector(const XMFLOAT3& InRight);
	virtual void SetUpVector(const XMFLOAT3& InUp);

	XMVECTOR GetRotationQuat() const;	// 获取旋转四元数 DXMath
	frotator GetRotationFRotator() const;	// 获取旋转信息 frotator
	fquat GetRotationFQuat() const;	// 获取旋转信息 fquat


	void SetRotationQuat(const XMVECTOR& InQuat);	// 设置旋转四元数
	void SetRotationFQuat(const fquat& quat);	// 设置旋转信息 fquat

	FORCEINLINE XMFLOAT3& GetPosition() { return Position; }
	FORCEINLINE XMFLOAT3& GetScale() { return Scale; }
	FORCEINLINE XMFLOAT3& GetForwardVector() { return ForwardVector; }
	FORCEINLINE XMFLOAT3& GetRightVector() { return RightVector; }
	FORCEINLINE XMFLOAT3& GetUpVector() { return UpVector; }

public:
	// 校正 Up, Right, Look(Forward)
	void CorrectionVector();

	fvector_3d GetCorrectionPosition();
	void GetCorrectionPosition(fvector_3d& InPos3);

	XMFLOAT4X4 CalculateViewMatrix();

private:

	XMFLOAT3 Position;			// 位置信息
	XMFLOAT3 Scale;				// 缩放信息

	XMFLOAT3 ForwardVector;		// forward	正方向向量 等同于龙书里的look向量
	XMFLOAT3 RightVector;		// right	右方向向量
	XMFLOAT3 UpVector;			// up		上方向向量
};

