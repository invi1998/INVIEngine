#pragma once
#include "CameraType.h"
#include "Actor/Core/ActorObject.h"
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
    void OnUpdate(float ts);

    FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }

    // XMVECTOR GetPosition() const { return Position; }

    // ��Ĭ�ϵ�ǰ���Ϸ����ҷ�������תΪ��ǰ����
    XMVECTOR GetUpDirection();
    XMVECTOR GetRightDirection();
    XMVECTOR GetForwardDirection();

    // void SetPosition(const XMVECTOR& position) { Position = position; }
    // void SetPosition(float x, float y, float z) { Position = XMVectorSet(x, y, z, 1.0f); }

    // XMMATRIX GetViewMatrix() const { return ViewMatrix; }

protected:
    void UpdateViewMatrix();
    
    XMVECTOR GetRotationQuaternion() const;
    
    void SetViewportSize(int width, int height);
    void OnMouseScroll(int X, int Y, float InDelta);

    void MouseRotate(const XMFLOAT2& delta);
    void MousePan(const XMFLOAT2& delta);
    void MousePanEdit(const XMFLOAT2& delta);
    void MouseZoom(float delta);

	void MoveForward(float delta);
	void MoveRight(float delta);

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

	void RotateAroundXAxis(float rotateDegrees);
	void RotateAroundYAxis(float rotateDegrees);

    XMVECTOR CalculatePosition();

private:
    /*static const XMVECTOR DefaultForward;
    static const XMVECTOR DefaultUp;
    static const XMVECTOR DefaultRight;*/

    // XMVECTOR Position;
    // XMVECTOR FocalPoint;
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;
    // XMMATRIX ViewMatrix{};
    // XMMATRIX XMProjectionMatrix{};

	float Distance = 10.0f;

	XMFLOAT2 m_InitialMousePosition = { 0.0f, 0.0f };

    ECameraType CameraType = ECameraType::CameraRoaming;        // Ĭ������ģʽ

private:
    float Radius = 10.f;
    float Theta = XM_PI;
    float Phi = XM_PI * 0.25f;

private:
    CVARIABLE();
    CInputComponent* InputComponent;
};

