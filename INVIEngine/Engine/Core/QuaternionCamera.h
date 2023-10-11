#pragma once
#include "CameraType.h"
#include "Actor/Core/ActorObject.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Viewport/ViewPort.h"

class CInputComponent;
class CTransformationComponent;

class GQuaternionCamera : public GActorObject, public FViewPort, public IDirectXDeviceInterface
{

public:
    GQuaternionCamera();
    ~GQuaternionCamera() override = default;

    virtual void BeginInit() override;
    virtual void Tick(float DeltaTime) override;

    virtual void ExecuteInput();
    void OnUpdate(float ts);

    FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }

    XMVECTOR GetPosition() const { return Position; }

    // 将默认的前向、上方和右方向量旋转为当前朝向。
    XMVECTOR GetUpDirection() const;
    XMVECTOR GetRightDirection() const;
    XMVECTOR GetForwardDirection() const;

    void SetPosition(const XMVECTOR& position) { Position = position; }
    void SetPosition(float x, float y, float z) { Position = XMVectorSet(x, y, z, 1.0f); }

    XMMATRIX GetViewMatrix() const { return ViewMatrix; }
    XMMATRIX GetProjectionMatrix() const { return ProjectionMatrix; }

    XMFLOAT4X4 GetViewMatrixFx4() const;
    XMFLOAT4X4 GetProjectionMatrixFx4() const;

protected:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float aspectRatio);
    XMVECTOR GetRotationQuaternion() const;
    
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

    XMVECTOR CalculatePosition() const;

private:
    static const XMVECTOR DefaultForward;
    static const XMVECTOR DefaultUp;
    static const XMVECTOR DefaultRight;

    XMVECTOR Position;
    XMVECTOR FocalPoint;
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;
    XMMATRIX ViewMatrix;
    XMMATRIX ProjectionMatrix;

    int ViewportWidth = 800;
    int ViewportHeight = 600;

	float Distance = 10.0f;
	float FOV = XM_PIDIV4;
	float NearPlane = 0.1f;
	float FarPlane = 1000.0f;

	XMFLOAT2 m_InitialMousePosition = { 0.0f, 0.0f };

    ECameraType CameraType = ECameraType::CameraRoaming;        // 默认漫游模式

private:
    float Radius = 10.f;
    float Theta = XM_PI;
    float Phi = XM_PI * 0.25f;

private:
    CVARIABLE();
    CInputComponent* InputComponent;
};

