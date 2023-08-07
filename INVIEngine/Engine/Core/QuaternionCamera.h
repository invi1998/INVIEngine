#pragma once
#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Viewport/ViewPort.h"

class CInputComponent;
class CTransformationComponent;

class CQuaternionCamera : public CCoreMinimalObject, public FViewPort, public IDirectXDeviceInterface
{
public:
    CQuaternionCamera();
    ~CQuaternionCamera() override = default;

    virtual void BeginInit() override;
    virtual void Tick(float DeltaTime) override;

    virtual void ExecuteInput();
    void OnUpdate(float ts);
    void OnEvent();

    void BuildViewMatrix();

    FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }
    FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }

    XMVECTOR GetPosition() const { return Position; }
    void SetPosition(const XMVECTOR& position) { Position = position; }
    void SetPosition(float x, float y, float z) { Position = XMVectorSet(x, y, z, 1.0f); }

    XMMATRIX GetViewMatrix() const { return ViewMatrix; }
    XMMATRIX GetProjectionMatrix() const { return ProjectionMatrix; }

    XMFLOAT4X4 GetViewMatrixFx4() const;
    XMFLOAT4X4 GetProjectionMatrixFx4() const;

private:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float aspectRatio);
    void UpdateRotationQuaternion();
    
    void SetViewportSize(int width, int height);
    void OnMouseScroll(int X, int Y, float InDelta);

private:
    static const XMVECTOR DefaultForward;
    static const XMVECTOR DefaultUp;
    static const XMVECTOR DefaultRight;

    XMVECTOR Position;
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;
    XMVECTOR RotationQuaternion;
    XMMATRIX ViewMatrix;
    XMMATRIX ProjectionMatrix;

    int ViewportWidth = 800;
    int ViewportHeight = 600;
    const float FOV = XM_PIDIV4;
    const float NearPlane = 0.1f;
    const float FarPlane = 1000.0f;

	XMFLOAT2 m_InitialMousePosition = { 0.0f, 0.0f };

private:
    CVARIABLE();
    CInputComponent* InputComponent;

    CVARIABLE();
    CTransformationComponent* TransformationComponent;
};

