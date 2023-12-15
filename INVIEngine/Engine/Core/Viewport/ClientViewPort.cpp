#include "EngineMinimal.h"
#include "ClientViewPort.h"

#include "Component/TransformationComponent.h"
#include "Config/EngineRenderConfig.h"

GClientViewPort::GClientViewPort()
	: ViewportWidth(FEngineRenderConfig::GetRenderConfig()->ScreenWidth),
	ViewportHeight(FEngineRenderConfig::GetRenderConfig()->ScreenHeight)
{
}

void GClientViewPort::Tick(float DeltaTime)
{
	GActorObject::Tick(DeltaTime);

	BuildViewMatrix();

}

void GClientViewPort::BuildViewMatrix()
{
	ViewMatrix = GetTransformationComponent()->CalculateViewMatrix();
}

void GClientViewPort::BuildOrthographicOffCenterProjectionMatrixLH(float radius, const XMFLOAT3& centerPos)
{
	XMVECTOR centerPosVEC = XMLoadFloat3(&centerPos);

	XMMATRIX ShadowViewMatrixMTX = XMLoadFloat4x4(&ViewMatrix);

	// ��Ŀ��λ��תΪ��Դ�ռ������
	XMFLOAT3 ViewCenter{};
	XMStoreFloat3(&ViewCenter, XMVector3TransformCoord(centerPosVEC, ShadowViewMatrixMTX));

	float ViewLeft = ViewCenter.x - radius;
	float ViewRight = ViewCenter.x + radius;
	float ViewBottom = ViewCenter.y - radius;
	float ViewTop = ViewCenter.y + radius;
	float NearZ = ViewCenter.z - radius;
	float FarZ = ViewCenter.z + radius;

	// ������������
	XMMATRIX projectionMatrixMTX = XMMatrixOrthographicOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);

	XMStoreFloat4x4(&ProjectionMatrix, projectionMatrixMTX);
}

void GClientViewPort::FaceTarget(const XMFLOAT3& position, const XMFLOAT3& targetPosition, const XMFLOAT3& upDirection)
{
	XMFLOAT3 TempFaceVector{};
	XMStoreFloat3(&TempFaceVector, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetPosition), XMLoadFloat3(&position))));

	XMFLOAT3 TempRightVector{};
	XMStoreFloat3(&TempRightVector, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&TempFaceVector))));

	// У��uP
	XMFLOAT3 TempUpVector{};
	XMStoreFloat3(&TempUpVector, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&TempFaceVector), XMLoadFloat3(&TempRightVector))));

	SetForwardVector(TempFaceVector);
	SetRightVector(TempRightVector);
	SetUpVector(TempUpVector);

	SetDirty(true);
}

void GClientViewPort::SetFrustum(float fov, int width, int height, float zNear, float zFar)
{
	FOV = fov;
	ViewportWidth = width;
	ViewportHeight = height;
	NearPlane = zNear;
	FarPlane = zFar;

	// ʹ��͸��ͶӰ����ͶӰ����ʹ�õ�ǰ��߱ȡ����ü����Զ�ü��档
	UpdateProjectionMatrix();

	SetDirty(true);
}

void GClientViewPort::UpdateProjectionMatrix()
{
	// ʹ��͸��ͶӰ����ͶӰ����ʹ�õ�ǰ��߱ȡ����ü����Զ�ü��档
	XMStoreFloat4x4(&ProjectionMatrix, XMMatrixPerspectiveFovLH(FOV, static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight), NearPlane, FarPlane));
	SetDirty(true);
}

void GClientViewPort::SetDirty(bool isDirty)
{
	bDirty = isDirty;
}


