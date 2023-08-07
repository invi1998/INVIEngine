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
    RotationQuaternion(XMQuaternionIdentity()),
    ViewportWidth(FEngineRenderConfig::GetRenderConfig()->ScreenWidth),
    ViewportHeight(FEngineRenderConfig::GetRenderConfig()->ScreenHeight)
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
    InputComponent->OnMouseWheelDelegate.Bind(this, &CQuaternionCamera::OnMouseScroll);
    // 绑定键盘鼠标事件
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

        float dx = XMConvertToRadians(static_cast<float>(FInput::GetMouseX() - m_InitialMousePosition.x));
        float dy = XMConvertToRadians(static_cast<float>(FInput::GetMouseY() - m_InitialMousePosition.y));

        m_InitialMousePosition = { FInput::GetMouseX(), FInput::GetMouseY() };

        if (FInput::IsMouseButtonPressed(VK_LBUTTON))
        {
            // 鼠标左键
            Pitch += dy * 0.8f;;
            Yaw += dx * 0.8f;;

            if (Pitch > XM_PIDIV2 - 0.01f)
                Pitch = XM_PIDIV2 - 0.01f;
            else if (Pitch < -XM_PIDIV2 + 0.01f)
                Pitch = -XM_PIDIV2 + 0.01f;

            UpdateRotationQuaternion();

            UpdateViewMatrix();
            UpdateProjectionMatrix(static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight));
        }
        
    }
}

void CQuaternionCamera::OnEvent()
{

}

void CQuaternionCamera::BuildViewMatrix()
{
}

void CQuaternionCamera::UpdateViewMatrix()
{
    using namespace DirectX;
    XMVECTOR forward = XMVector3Rotate(DefaultForward, RotationQuaternion);
    forward = XMVector3Normalize(forward);
    XMVECTOR up = XMVector3Rotate(DefaultUp, RotationQuaternion);
    XMVECTOR right = XMVector3Cross(up, forward);

    XMMATRIX viewMatrix = XMMatrixIdentity();
    viewMatrix.r[0] = XMVectorSetW(right, -XMVectorGetX(XMVector3Dot(Position, right)));
    viewMatrix.r[1] = XMVectorSetW(up, -XMVectorGetX(XMVector3Dot(Position, up)));
    viewMatrix.r[2] = XMVectorSetW(forward, -XMVectorGetX(XMVector3Dot(Position, forward)));
    viewMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    ViewMatrix = viewMatrix;
}

void CQuaternionCamera::UpdateProjectionMatrix(float aspectRatio)
{
    using namespace DirectX;
    ProjectionMatrix = XMMatrixPerspectiveFovLH(FOV, static_cast<float>(ViewportWidth) / ViewportHeight, NearPlane, FarPlane);
}

void CQuaternionCamera::UpdateRotationQuaternion()
{
    using namespace DirectX;
    // 将欧拉角转换为四元数并设置Roll为0。
    XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(Pitch, Yaw, 0.0f);
    RotationQuaternion = quaternion;
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
    UpdateProjectionMatrix(static_cast<float>(width) / height);
}

void CQuaternionCamera::OnMouseScroll(int X, int Y, float InDelta)
{
    float zoom = static_cast<float>(InDelta) / WHEEL_DELTA;

    ENGINE_LOG("鼠标滚轮：%f", zoom);

    XMVECTOR forward = XMVector3Rotate(DefaultForward, RotationQuaternion);
    forward = XMVector3Normalize(forward);
    Position += forward * zoom;

    UpdateRotationQuaternion();
    UpdateViewMatrix();
    UpdateProjectionMatrix(static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight));
}
