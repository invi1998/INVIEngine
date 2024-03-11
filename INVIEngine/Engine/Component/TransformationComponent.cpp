#include "EngineMinimal.h"

#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	: Position(0.f, 0.f, 0.f)
	, Rotation(0.f, 0.f, 0.f)
	, Scale(1.f, 1.f, 1.f)
	, ForwardVector(0.f, 0.f, 1.f)
	, RightVector(1.f, 0.f, 0.f)
	, UpVector(0.f, 1.f, 0.f)
{
}

CTransformationComponent::~CTransformationComponent()
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3& InPosition)
{
	Position = InPosition;
}

void CTransformationComponent::SetRotation(const XMFLOAT3& InRotation)
{
	// ��ȡ��һ�ε���ת�Ƕ�
	XMVECTOR LastRotation = XMLoadFloat3(&Rotation);
	// ������תƫ��
	XMVECTOR RotationOffset = XMLoadFloat3(&InRotation) - LastRotation;

	XMFLOAT3 rotation{};
	XMStoreFloat3(&rotation, RotationOffset);

	// �������Ĳ�������ת�Ƕȣ���Ҫת�ɻ��Ƚ��м���
	float rollRadians = XMConvertToRadians(rotation.z);
	float pitchRadians = XMConvertToRadians(rotation.x);
	float yawRadians = XMConvertToRadians(rotation.y);

	// Rotation = XMFLOAT3(pitchRadians, yawRadians, rollRadians);

	// �����ת����
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);

	// �õ�3����������
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	// �÷�������������ת���󣬵õ���ת�任�������
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationRollPitchYawMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(Up, RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationRollPitchYawMatrix));

	XMStoreFloat3(&Rotation, LastRotation);		// ������ת�Ƕ�
}

void CTransformationComponent::SetRotation(const frotator& InRotation)
{
	float rollRadians = XMConvertToRadians(InRotation.roll);
	float pitchRadians = XMConvertToRadians(InRotation.pitch);
	float yawRadians = XMConvertToRadians(InRotation.yaw);

	// ��ת����
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);

	// ��һ���õ�3����������
	XMVECTOR Right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR Forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationRollPitchYawMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(Up, RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationRollPitchYawMatrix));

	// ������ת�Ƕȣ���Ϊ�������frotator��ʱ��roll��x�ᣬpitch��y�ᣬyaw��z�ᣬ��������Ҫת��һ�£�����ֱ��yaw, pitch, roll
	Rotation = { InRotation.yaw, InRotation.pitch, InRotation.roll };

}

void CTransformationComponent::SetScale(const XMFLOAT3& InScale)
{
	Scale = InScale;
}

void CTransformationComponent::SetForwardVector(const XMFLOAT3& InForward)
{
	ForwardVector = InForward;
}

void CTransformationComponent::SetRightVector(const XMFLOAT3& InRight)
{
	RightVector = InRight;
}

void CTransformationComponent::SetUpVector(const XMFLOAT3& InUp)
{
	UpVector = InUp;
}

XMFLOAT3& CTransformationComponent::GetRotation()
{
	/*float x = XMConvertToDegrees(Rotation.x);
	float y = XMConvertToDegrees(Rotation.y);
	float z = XMConvertToDegrees(Rotation.z);

	XMFLOAT3 rotation = { x, y, z };*/
	return Rotation;
}

XMVECTOR CTransformationComponent::GetRotationQuat() const
{
	XMMATRIX RotationMatrix{};
	EngineMath::BuildRotationMatrix(RotationMatrix, RightVector, UpVector, ForwardVector);

	return XMQuaternionRotationMatrix(RotationMatrix);
}

void CTransformationComponent::SetRotationQuat(const XMVECTOR& InQuat)
{
	XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(InQuat);	// ͨ����Ԫ���õ���ת����

	XMVECTOR Right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR Forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationMatrix));		// ��˽���õ���������Ȼ���һ��
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(Up, RotationMatrix));		// ��˽���õ���������Ȼ���һ��
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationMatrix));		// ��˽���õ�ǰ������Ȼ���һ��
}

void CTransformationComponent::CorrectionVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));		// ��˽���õ���������Ȼ���һ��
	Right = XMVector3Cross(Up, Forward);		// ��˽���õ���������Ȼ���һ��

	XMStoreFloat3(&ForwardVector, Forward);
	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UpVector, Up);
}

fvector_3d CTransformationComponent::GetCorrectionPosition()
{
	fvector_3d V3;
	GetCorrectionPosition(V3);
	return V3;
}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& InPos3)
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMVECTOR NewPosition = XMLoadFloat3(&Position);

	InPos3.x = -XMVectorGetX(XMVector3Dot(NewPosition, Right));
	InPos3.y = -XMVectorGetX(XMVector3Dot(NewPosition, Up));
	InPos3.z = -XMVectorGetX(XMVector3Dot(NewPosition, Forward));
}

XMFLOAT4X4 CTransformationComponent::CalculateViewMatrix()
{
	fvector_3d V3;
	// �����У����
	CorrectionVector();

	GetCorrectionPosition(V3);

	XMFLOAT4X4 ViewMatrix = {
		RightVector.x,	UpVector.x, ForwardVector.x, 0.f,
		RightVector.y,	UpVector.y, ForwardVector.y, 0.f,
		RightVector.z,	UpVector.z, ForwardVector.z, 0.f,
		V3.x,			V3.y,		V3.z,			1.f,
	};

	return ViewMatrix;
}
