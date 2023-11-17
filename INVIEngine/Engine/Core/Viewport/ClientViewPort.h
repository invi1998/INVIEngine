#pragma once
#include "ViewPort.h"
#include "Actor/Core/ActorObject.h"


class GClientViewPort : public GActorObject, public FViewPort
{
public:
	GClientViewPort();

	virtual void Tick(float DeltaTime) override;

	void BuildViewMatrix();

	// 计算面朝向
	void FaceTarget(const XMFLOAT3& position, const XMFLOAT3& targetPosition, const XMFLOAT3& upDirection = {0, 1.f, 0});

	// 设置视椎体
	void SetFrustum(float fov, int width, int height, float zNear, float zFar);

	void UpdateProjectionMatrix();

	void SetDirty(bool isDirty);

	XMFLOAT4X4 GetViewMatrix() const { return ViewMatrix; };
	XMFLOAT4X4 GetProjectionMatrix() const { return ProjectionMatrix; };

public:
	FORCEINLINE bool IsDirty() const { return bDirty; }

protected:
	int ViewportWidth = 800;
	int ViewportHeight = 600;
	float FOV = XM_PIDIV4;
	float NearPlane = 0.1f;
	float FarPlane = 10000.0f;

	bool bDirty=true;
};

