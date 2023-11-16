#include "EngineMinimal.h"
#include "ClientViewPort.h"

FClientViewPort::FClientViewPort()
{
}

bool FClientViewPort::FaceTarget(const XMFLOAT3& position, const XMFLOAT3& targetPosition, const XMFLOAT3& upDirection)
{
	XMFLOAT3 faceVector{};
	XMStoreFloat3(&faceVector, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetPosition), XMLoadFloat3(&position))));


}

void FClientViewPort::SetFrustum(float fov, float aspectRatio, float zNear, float zFar)
{
}


