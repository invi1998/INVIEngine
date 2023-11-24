#include "EngineMinimal.h"

#include "QuaternionCamera.h"

#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"
#include "Config/EngineRenderConfig.h"

FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;

const XMVECTOR GQuaternionCamera::DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR GQuaternionCamera::DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR GQuaternionCamera::DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

GQuaternionCamera::GQuaternionCamera()
    : GClientViewPort(),
	// Position(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    FocalPoint(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    InputComponent->OnMouseWheelDelegate.Bind(this, &GQuaternionCamera::OnMouseScroll);
    // �󶨼�������¼�
    InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &GQuaternionCamera::ExecuteInput);

    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void GQuaternionCamera::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void GQuaternionCamera::Tick(float DeltaTime)
{
	GClientViewPort::Tick(DeltaTime);
}

void GQuaternionCamera::ExecuteInput()
{
    OnUpdate(1.0f);
}

void GQuaternionCamera::OnUpdate(float ts)
{
	
    if (FInput::IsKeyPressed(VK_LMENU))
    {
        const XMFLOAT2& mouse{ FInput::GetMouseX(), FInput::GetMouseY() };
        XMFLOAT2 delta = { (mouse.x - m_InitialMousePosition.x) * 0.003f, (mouse.y - m_InitialMousePosition.y) * 0.003f };
        m_InitialMousePosition = mouse;

        if (FInput::IsMouseButtonPressed(VK_LBUTTON))
        {
            // ������
            MouseRotate(delta);
        }
        else if (FInput::IsMouseButtonPressed(VK_MBUTTON))
        {
            // ����м�
            MousePan(delta);
        }
        else if (FInput::IsMouseButtonPressed(VK_RBUTTON))
        {
            // ����Ҽ�
            MouseZoom(delta.y);
        }
    }
    if (FInput::IsKeyReleased(VK_TAB))
    {
        CameraType = CameraType == ECameraType::CameraRoaming ? ECameraType::ObservationObject : CameraRoaming;
    }
    if (FInput::IsKeyReleased(Key::W))
    {
		MoveForward(2.f);
    }
	if (FInput::IsKeyReleased(Key::S))
	{
		MoveForward(-2.f);
	}
	if (FInput::IsKeyReleased(Key::A))
	{
		MoveRight(2.f);
	}
	if (FInput::IsKeyReleased(Key::D))
	{
		MoveRight(-2.f);
	}
	if (FInput::IsKeyReleased(Key::E))
	{
		XMFLOAT2 delta = { 2.f, 0.f};
		MouseRotate(delta);
	}
	if (FInput::IsKeyReleased(Key::Q))
	{
		XMFLOAT2 delta = { -2.f, 0.f };
		MouseRotate(delta);
	}

	UpdateViewMatrix();
}

XMVECTOR GQuaternionCamera::GetUpDirection() const
{
    XMVECTOR up = XMVector3Rotate(DefaultUp, GetRotationQuaternion());
    // �����������й�һ����
    up = XMVector3Normalize(up);
    return up;
}

XMVECTOR GQuaternionCamera::GetRightDirection() const
{
    XMVECTOR right = XMVector3Rotate(DefaultRight, GetRotationQuaternion());
    // �����������й�һ����
	right = XMVector3Normalize(right);
    return right;
}

XMVECTOR GQuaternionCamera::GetForwardDirection() const
{
    XMVECTOR forward = XMVector3Rotate(DefaultForward, GetRotationQuaternion());
    // ��ǰ�������й�һ����
    forward = XMVector3Normalize(forward);
    return forward;
}

XMVECTOR GQuaternionCamera::CalculatePosition() const
{
    return FocalPoint + GetForwardDirection() * Distance;
}

void GQuaternionCamera::UpdateViewMatrix()
{
	XMVECTOR Position = CalculatePosition();
	XMFLOAT4 position{};
	XMStoreFloat4(&position, CalculatePosition());
	SetPosition({ position.x, position.y, position.z });

	switch (CameraType)
	{
	case CameraRoaming:
		{
	        // m_Yaw = m_Pitch = 0.0f;	// ���������ת
			XMVECTOR orientation = GetRotationQuaternion();
	        XMMATRIX rotation = XMMatrixRotationQuaternion(orientation);
	        XMMATRIX transform = XMMatrixTranslationFromVector(Position) * rotation;

			XMStoreFloat4x4(&ViewMatrix, transform);

            break;
		}
	case ObservationObject:
		{
	        XMFLOAT3 CameraPosition{};
	        XMStoreFloat3(&CameraPosition, Position);
	        CameraPosition.x = Radius * sinf(Phi) * cosf(Theta);
	        CameraPosition.z = Radius * sinf(Phi) * sinf(Theta);
	        CameraPosition.y = Radius * cosf(Phi);

            XMVECTOR Pos = XMVectorSet(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f);
            XMVECTOR ViewTarget = XMVectorZero();
            XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

            XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);

			XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

            Position = XMLoadFloat3(&CameraPosition);

            break;
		}
	}

	SetDirty(true);
}

XMVECTOR GQuaternionCamera::GetRotationQuaternion() const
{
    // ��ŷ����ת��Ϊ��Ԫ��������RollΪ0��
	XMVECTOR rotation = XMVectorSet(math_utils::angle_to_radian(Pitch), math_utils::angle_to_radian(Yaw), 0.f, 0.f);
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYawFromVector(rotation);
    // XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(quaternionRotation);
    return quaternion;
}

void GQuaternionCamera::SetViewportSize(int width, int height)
{
    ViewportWidth = width;
    ViewportHeight = height;
    UpdateProjectionMatrix();
}

void GQuaternionCamera::OnMouseScroll(int X, int Y, float InDelta)
{
    float zoom = InDelta / WHEEL_DELTA;

    MouseZoom(zoom);
    UpdateViewMatrix();
    
}

void GQuaternionCamera::MouseRotate(const XMFLOAT2& delta)
{
	switch (CameraType) {
		case CameraRoaming:
			{
	            XMFLOAT3 up{};
	            XMStoreFloat3(&up, GetUpDirection());
				ENGINE_LOG_ERROR("UP DIRECTION = %f, %f, %f", up.x, up.y, up.z);
	            const float yawSign = up.y < 0 ? -1.0f : 1.0f;
	            Yaw += yawSign * delta.x * RotationSpeed();
	            Pitch += delta.y * RotationSpeed();
				MousePanEdit(delta);

                break;
			}
	case ObservationObject:
			{
				float XRadians = XMConvertToRadians(delta.x * 10.f);
				float YRadians = XMConvertToRadians(delta.y * 10.f);
				
                Theta += -XRadians;
                Phi += YRadians;

                break;
			}
	}
    
}

void GQuaternionCamera::MousePan(const XMFLOAT2& delta)
{
    auto [xSpeed, ySpeed] = PanSpeed();
    FocalPoint += -GetRightDirection() * delta.x * xSpeed * Distance;
    FocalPoint += GetUpDirection() * delta.y * ySpeed * Distance;
}

void GQuaternionCamera::MousePanEdit(const XMFLOAT2& delta)
{
	auto [xSpeed, ySpeed] = PanSpeed();
	FocalPoint += GetRightDirection() * delta.x * xSpeed * Distance;
	FocalPoint += -GetUpDirection() * delta.y * ySpeed * Distance;
}

void GQuaternionCamera::MouseZoom(float delta)
{
    if (CameraType == ObservationObject)
    {
        Radius += delta * ZoomSpeed();
    }
	else
    {
		Distance -= delta * ZoomSpeed();
    }
   
}

void GQuaternionCamera::MoveForward(float delta)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		// FocalPoint += GetForwardDirection();
		Distance -= delta;
	}
}

void GQuaternionCamera::MoveRight(float delta)
{
	if (CameraType == ECameraType::CameraRoaming)
	{
		// FocalPoint += -GetRightDirection() * delta.x * xSpeed * Distance;
		FocalPoint += GetRightDirection() * delta;
	}
}

std::pair<float, float> GQuaternionCamera::PanSpeed() const
{
    float x = min(ViewportWidth / 1000.0f, 2.4f);	// max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = min(ViewportHeight / 1000.0f, 2.4f);	// max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { xFactor, yFactor };
}

float GQuaternionCamera::RotationSpeed() const
{
    return 0.8f;
}

float GQuaternionCamera::ZoomSpeed() const
{
	float distance = Distance * 0.2f;
	distance = max(distance, 0.2f);
	float speed = distance * distance;
	speed = min(speed, 50.0f); // max speed = 100
	return speed;
}
