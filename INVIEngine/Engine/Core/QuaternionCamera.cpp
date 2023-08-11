#include "QuaternionCamera.h"

#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"
#include "Config/EngineRenderConfig.h"

FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;

const XMVECTOR CQuaternionCamera::DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR CQuaternionCamera::DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR CQuaternionCamera::DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

CQuaternionCamera::CQuaternionCamera()
    : Position(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    FocalPoint(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    ViewportWidth(FEngineRenderConfig::GetRenderConfig()->ScreenWidth),
    ViewportHeight(FEngineRenderConfig::GetRenderConfig()->ScreenHeight)
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
    InputComponent->OnMouseWheelDelegate.Bind(this, &CQuaternionCamera::OnMouseScroll);
    // �󶨼�������¼�
    InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &CQuaternionCamera::ExecuteInput);

    UpdateViewMatrix();
    UpdateProjectionMatrix(static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight));
}

void CQuaternionCamera::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CQuaternionCamera::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CQuaternionCamera::ExecuteInput()
{
    OnUpdate(1.0f);
}

void CQuaternionCamera::OnUpdate(float ts)
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

        UpdateViewMatrix();
    }
    if (FInput::IsKeyReleased(VK_TAB))
    {
        CameraType = CameraType == ECameraType::CameraRoaming ? ECameraType::ObservationObject : CameraRoaming;

        UpdateViewMatrix();
    }
}

XMVECTOR CQuaternionCamera::GetUpDirection() const
{
    XMVECTOR up = XMVector3Rotate(DefaultUp, GetRotationQuaternion());
    // �����������й�һ����
    up = XMVector3Normalize(up);
    return up;
}

XMVECTOR CQuaternionCamera::GetRightDirection() const
{
    XMVECTOR right = XMVector3Rotate(DefaultRight, GetRotationQuaternion());
    // �����������й�һ����
	right = XMVector3Normalize(right);
    return right;
}

XMVECTOR CQuaternionCamera::GetForwardDirection() const
{
    XMVECTOR forward = XMVector3Rotate(DefaultForward, GetRotationQuaternion());
    // ��ǰ�������й�һ����
    forward = XMVector3Normalize(forward);
    return forward;
}

XMVECTOR CQuaternionCamera::CalculatePosition() const
{
    return FocalPoint + GetForwardDirection() * Distance;
}

void CQuaternionCamera::UpdateViewMatrix()
{
    Position = CalculatePosition();

	switch (CameraType)
	{
	case CameraRoaming:
		{
	        // m_Yaw = m_Pitch = 0.0f;	// ���������ת
			XMVECTOR orientation = GetRotationQuaternion();
	        XMMATRIX rotation = XMMatrixRotationQuaternion(orientation);
	        XMMATRIX transform = XMMatrixTranslationFromVector(Position) * rotation;

	        ViewMatrix = transform;

            break;
		};
	case ObservationObject:
		{
	        XMFLOAT3 CameraPosition;
	        XMStoreFloat3(&CameraPosition, Position);
	        CameraPosition.x = Radius * sinf(Phi) * cosf(Theta);
	        CameraPosition.z = Radius * sinf(Phi) * sinf(Theta);
	        CameraPosition.y = Radius * cosf(Phi);

            XMVECTOR Pos = XMVectorSet(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f);
            XMVECTOR ViewTarget = XMVectorZero();
            XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

            XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
            ViewMatrix = ViewLookAt;

            break;
		};
	}
}

void CQuaternionCamera::UpdateProjectionMatrix(float aspectRatio)
{
    // ʹ��͸��ͶӰ����ͶӰ����ʹ�õ�ǰ��߱ȡ����ü����Զ�ü��档
    ProjectionMatrix = XMMatrixPerspectiveFovLH(FOV, static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight), NearPlane, FarPlane);
}

XMVECTOR CQuaternionCamera::GetRotationQuaternion() const
{
    // ��ŷ����ת��Ϊ��Ԫ��������RollΪ0��
    XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(Pitch, Yaw, 0.0f);
    return quaternion;
}

XMFLOAT4X4 CQuaternionCamera::GetViewMatrixFx4() const
{
    XMFLOAT4X4 View;
    XMStoreFloat4x4(&View, ViewMatrix);

    return View;
}

XMFLOAT4X4 CQuaternionCamera::GetProjectionMatrixFx4() const
{
    XMFLOAT4X4 Projection;
    XMStoreFloat4x4(&Projection, ProjectionMatrix);

    return Projection;
}

void CQuaternionCamera::SetViewportSize(int width, int height)
{
    ViewportWidth = width;
    ViewportHeight = height;
    UpdateProjectionMatrix(static_cast<float>(width) / static_cast<float>(height));
}

void CQuaternionCamera::OnMouseScroll(int X, int Y, float InDelta)
{
    float zoom = InDelta / WHEEL_DELTA;

    MouseZoom(zoom);
    UpdateViewMatrix();
    
}

void CQuaternionCamera::MouseRotate(const XMFLOAT2& delta)
{
	switch (CameraType) {
		case CameraRoaming:
			{
	            XMFLOAT3 up;
	            XMStoreFloat3(&up, GetUpDirection());
	            float yawSign = up.y < 0 ? -1 : 1.0f;
	            Yaw += yawSign * delta.x * RotationSpeed();
	            Pitch += delta.y * RotationSpeed();

                break;
			};
		case ObservationObject:
			{
				float XRadians = XMConvertToRadians(delta.x * 10.f);
				float YRadians = XMConvertToRadians(delta.y * 10.f);

                Theta += -XRadians;
                Phi += YRadians;

                Theta = XMScalarModAngle(Theta);
                Phi = XMScalarModAngle(Phi);

                break;
			};
	}
    
}

void CQuaternionCamera::MousePan(const XMFLOAT2& delta)
{
    auto [xSpeed, ySpeed] = PanSpeed();
    FocalPoint += -GetRightDirection() * delta.x * xSpeed * Distance;
    FocalPoint += GetUpDirection() * delta.y * ySpeed * Distance;
}

void CQuaternionCamera::MouseZoom(float delta)
{
    if (CameraType == ObservationObject)
    {
        Radius += (delta * ZoomSpeed());
    }
	else
    {
    	Distance -= delta * ZoomSpeed();
	    if (Distance < 1.0f)
	    {
	        FocalPoint += GetForwardDirection();
	        Distance = 1.0f;
	    }
    }
   
}

std::pair<float, float> CQuaternionCamera::PanSpeed() const
{
    float x = min(ViewportWidth / 1000.0f, 2.4f);	// max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = min(ViewportHeight / 1000.0f, 2.4f);	// max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { xFactor, yFactor };
}

float CQuaternionCamera::RotationSpeed() const
{
    return 0.8f;
}

float CQuaternionCamera::ZoomSpeed() const
{
    float distance = Distance * 0.2f;
    distance = max(distance, 0.0f);
    float speed = distance * distance;
    speed = min(speed, 100.0f); // max speed = 100
    return speed;
}
