#pragma once

#include "EngineMinimal.h"

#include "ColdeReflection/CodeReflectionMacro.h"
#include "CoreObject/CoreMinimalObject.h"
#include "Component/Input/Input.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Viewport/ViewPort.h"

class CInputComponent;
class CTransformationComponent;

class CCamera : public CCoreMinimalObject, public FViewPort, public IDirectXDeviceInterface
{
public:
	CCamera();
	CCamera(float fov, float aspectRatio, float nearClip, float farClip);
	~CCamera() override;

	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInput();

	void BuildViewMatrix();

public:
	inline float GetDistance() const { return m_Distance; }
	inline void SetDistance(float distance) { m_Distance = distance; }

	inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

	const XMFLOAT4X4& GetViewMatrix() const { return m_ViewMatrix; }
	XMFLOAT4X4 GetViewProjection() const;

	XMFLOAT3 GetUpDirection() const;
	XMFLOAT3 GetRightDirection() const;
	XMFLOAT3 GetForwardDirection() const;

	const XMFLOAT3& GetPosition() const { return m_Position; }
	XMFLOAT4 GetOrientation() const;

	float GetPitch() const { return m_Pitch; }
	float GetYaw() const { return m_Yaw; }

private:
	void UpdateProjection();
	void UpdateView();

	bool OnMouseScroll();

	void MousePan(const XMFLOAT2& delta);
	void MouseRotate(const XMFLOAT2& delta);
	void MouseZoom(float delta);

	XMFLOAT3 CalculatePosition() const;

	std::pair<float, float> PanSpeed() const;
	float RotationSpeed() const;
	float ZoomSpeed() const;

private:
	float m_FOV = 45.0f;
	float m_AspectRatio = 1.778f;
	float m_NearClip = 0.1f;
	float m_FarClip = 1000.0f;

	XMFLOAT4X4 m_ViewMatrix;
	XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

	XMFLOAT2 m_InitialMousePosition = { 0.0f, 0.0f };

	float m_Distance = 10.0f;
	float m_Pitch = 0.0f;
	float m_Yaw = 0.0f;

	float m_ViewportWidth = 1280.0f;
	float m_ViewportHeight = 720.0f;


public:
	virtual void OnMouseMove(const XMFLOAT2& delta);

	virtual void MoveForward(float InValue);
	virtual void MoveRight(float InValue);

public:

	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }

protected:
	void RotateAroundYAxis(float InRotateDegrees);		// »∆Y÷·–˝◊™
	void RotateAroundZAxis(float InRotateDegrees);		// »∆Z÷·–˝◊™

	float MouseSensitivity = 0.003f;		//  Û±Í¡È√Ù∂»

private:
	CVARIABLE();
	CInputComponent* InputComponent;

	CVARIABLE();
	CTransformationComponent* TransformationComponent;

protected:
	XMFLOAT2 LastMousePosition = {0.f, 0.f};
	bool bIsLeftMouseDown = false;
};

