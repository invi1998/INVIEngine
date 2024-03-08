#pragma once
#include "CameraType.h"
#include "Actor/Core/ActorObject.h"
#include "Component/TimelineComponent.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Viewport/ClientViewPort.h"
#include "Viewport/ViewPort.h"

class CInputComponent;
class CTransformationComponent;

class GQuaternionCamera : public GClientViewPort, public IDirectXDeviceInterface
{

public:
    GQuaternionCamera();
    ~GQuaternionCamera() override = default;

    virtual void BeginInit() override;
    virtual void Tick(float DeltaTime) override;

    virtual void ExecuteInput();
    void MouseStrafe(const XMFLOAT2& delta);
    void OnUpdate(float ts);

    FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }

protected:
    // void UpdateViewMatrix();
    
    void SetViewportSize(int width, int height);
    void OnMouseScroll(int X, int Y, float InDelta);

    void MouseRotate(const XMFLOAT2& delta);
    void MousePan(const XMFLOAT2& delta);
    void MouseZoom(float delta);

	void MoveForward(float delta);
	void MoveRight(float delta);

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

	void RotateAroundXAxis(float rotateDegrees);
	void RotateAroundYAxis(float rotateDegrees);

	// 射线检测
	void OnClickScene(const XMFLOAT2& mousePos);

	void BuildMatrixByType();

	void LookAtAndMoveToSelectedObject(float currentTime, float duration);

private:
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;

	POINT m_InitialMousePosition;

    ECameraType CameraType = ECameraType::CameraRoaming;        // 默认漫游模式

private:
    float Radius = 10.f;
    float Theta = XM_PI;
    float Phi = XM_PI * 0.25f;

private:
    CVARIABLE();
    CInputComponent* InputComponent;

	FTimeline CameraTimeline;	// 时间轴
};

