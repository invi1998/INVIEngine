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
	// 获取上一次的旋转角度
	XMVECTOR LastRotation = XMLoadFloat3(&Rotation);
	// 计算旋转偏移
	XMVECTOR RotationOffset = XMLoadFloat3(&InRotation) - LastRotation;

	XMFLOAT3 rotation{};
	XMStoreFloat3(&rotation, RotationOffset);

	// 传进来的参数是旋转角度，需要转成弧度进行计算
	float rollRadians = XMConvertToRadians(rotation.z);
	float pitchRadians = XMConvertToRadians(rotation.x);
	float yawRadians = XMConvertToRadians(rotation.y);

	// Rotation = XMFLOAT3(pitchRadians, yawRadians, rollRadians);

	// 求出旋转矩阵
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);

	// 拿到3个方向向量
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	// 用方向向量乘以旋转矩阵，得到旋转变换后的向量
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationRollPitchYawMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(Up, RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationRollPitchYawMatrix));

	XMStoreFloat3(&Rotation, LastRotation);		// 保存旋转角度
}

void CTransformationComponent::SetRotation(const frotator& InRotation)
{
	float rollRadians = XMConvertToRadians(InRotation.roll);
	float pitchRadians = XMConvertToRadians(InRotation.pitch);
	float yawRadians = XMConvertToRadians(InRotation.yaw);

	// 旋转矩阵
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians);

	// 归一化拿到3个方向向量
	XMVECTOR Right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR Forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationRollPitchYawMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(Up, RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationRollPitchYawMatrix));

	// 保存旋转角度，因为我们设计frotator的时候，roll是x轴，pitch是y轴，yaw是z轴，所以这里要转换一下，不能直接yaw, pitch, roll
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
	XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(InQuat);	// 通过四元数得到旋转矩阵

	XMVECTOR Right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR Forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(Right, RotationMatrix));		// 叉乘结果得到右向量，然后归一化
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(Up, RotationMatrix));		// 叉乘结果得到上向量，然后归一化
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(Forward, RotationMatrix));		// 叉乘结果得到前向量，然后归一化
}

void CTransformationComponent::CorrectionVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));		// 叉乘结果得到上向量，然后归一化
	Right = XMVector3Cross(Up, Forward);		// 叉乘结果得到右向量，然后归一化

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
	// 计算和校正轴
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
