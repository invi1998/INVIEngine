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

void GClientViewPort::FaceTarget(const XMFLOAT3& position, const XMFLOAT3& targetPosition, const XMFLOAT3& upDirection)
{
	XMFLOAT3 TempFaceVector{};
	XMStoreFloat3(&TempFaceVector, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetPosition), XMLoadFloat3(&position))));

	XMFLOAT3 TempRightVector{};
	XMStoreFloat3(&TempRightVector, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&TempFaceVector))));

	// 校正uP
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

	// 使用透视投影创建投影矩阵，使用当前宽高比、近裁剪面和远裁剪面。
	UpdateProjectionMatrix();

	SetDirty(true);
}

void GClientViewPort::UpdateProjectionMatrix()
{
	// 使用透视投影创建投影矩阵，使用当前宽高比、近裁剪面和远裁剪面。
	XMStoreFloat4x4(&ProjectionMatrix, XMMatrixPerspectiveFovLH(FOV, static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight), NearPlane, FarPlane));
	SetDirty(true);
}

void GClientViewPort::SetDirty(bool isDirty)
{
	bDirty = isDirty;
}


