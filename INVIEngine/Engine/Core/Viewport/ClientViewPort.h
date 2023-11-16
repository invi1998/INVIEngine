#pragma once
#include "ViewPort.h"
#include "Actor/Core/ActorObject.h"


class FClientViewPort : public GActorObject, public FViewPort
{
public:
	FClientViewPort();

	// �����泯��
	bool FaceTarget(const XMFLOAT3& position, const XMFLOAT3& targetPosition, const XMFLOAT3& upDirection = {0, 1.f, 0});

	// ������׵��
	void SetFrustum(float fov, float aspectRatio, float zNear, float zFar);

private:
	float FOV = XM_PIDIV4;
	float NearPlane = 0.1f;
	float FarPlane = 10000.0f;
};

