#include "EditCamera.h"

#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"
#include "Config/EngineRenderConfig.h"

CEditCamera::CEditCamera()
	: RowCamera()
{
	m_AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

	XMStoreFloat4x4(&m_Projection, XMMatrixPerspectiveFovRH(XMConvertToRadians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip));

	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
	InputComponent->OnMouseWheelDelegate.Bind(this, &CEditCamera::OnMouseScroll);

	UpdateView();
}

CEditCamera::CEditCamera(float fov, float aspectRatio, float nearClip, float farClip)
	: RowCamera(), m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_ViewMatrix(EngineMath::IdentityMatrix4x4())
{
	m_AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

	XMStoreFloat4x4(&m_Projection, XMMatrixPerspectiveFovRH(XMConvertToRadians(fov), aspectRatio, nearClip, farClip));

	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
	InputComponent->OnMouseWheelDelegate.Bind(this, &CEditCamera::OnMouseScroll);

	UpdateView();
}

void CEditCamera::BeginInit()
{
	CCoreMinimalObject::BeginInit();

	// �󶨼�������¼�
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &CEditCamera::ExecuteInput);
}

void CEditCamera::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CEditCamera::ExecuteInput()
{
	OnUpdate(1.0f);
}

void CEditCamera::BuildViewMatrix()
{
}

void CEditCamera::OnUpdate(float ts)
{

	if (FInput::IsKeyPressed(VK_LMENU))
	{
		const XMFLOAT2& mouse{ FInput::GetMouseX(), FInput::GetMouseY() };
		XMFLOAT2 delta = { (mouse.x - m_InitialMousePosition.x) * 0.003f, (mouse.y - m_InitialMousePosition.y) * 0.003f };

		m_InitialMousePosition = mouse;

		if (FInput::IsMouseButtonPressed(VK_MBUTTON))
		{
			// ����м�
			MousePan(delta);
		}
		else if (FInput::IsMouseButtonPressed(VK_LBUTTON))
		{
			// ������
			MouseRotate(delta);
		}
		else if (FInput::IsMouseButtonPressed(VK_RBUTTON))
		{
			// ����Ҽ�
			MouseZoom(delta.y);
		}

		UpdateView();
	}
}

void CEditCamera::OnEvent()
{
}

XMFLOAT4X4 CEditCamera::GetViewProjection() const
{
	// return m_Projection * m_ViewMatrix;

	XMMATRIX projection = XMLoadFloat4x4(&m_Projection);
	XMMATRIX view = XMLoadFloat4x4(&m_ViewMatrix);

	XMFLOAT4X4 ret;
	XMStoreFloat4x4(&ret, XMMatrixMultiply(projection, view));

	return ret;
}

XMFLOAT3 CEditCamera::GetUpDirection() const
{
	// return glm::rotate(GetOrientation(), glm::vec3{ 1.0f, 1.0f, 0.0f });

	// ������ת����
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(GetOrientation());

	XMFLOAT3 UpDirection;

	XMFLOAT3 UpVector = { 0.0f, 1.0f, 0.0f };

	XMStoreFloat3(&UpDirection, XMVector3TransformNormal(XMLoadFloat3(&UpVector), rotationMatrix));

	return UpDirection;
}

XMFLOAT3 CEditCamera::GetRightDirection() const
{
	// return glm::rotate(GetOrientation(), glm::vec3{ 1.0f, 0.0f, 0.0f });

	// ������ת����
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(GetOrientation());

	XMFLOAT3 RightDirection;

	XMFLOAT3 RightVector = { 1.0f, 0.0f, 0.0f };

	XMStoreFloat3(&RightDirection, XMVector3TransformNormal(XMLoadFloat3(&RightVector), rotationMatrix));

	return RightDirection;

}

XMFLOAT3 CEditCamera::GetForwardDirection() const
{
	// return glm::rotate(GetOrientation(), glm::vec3{ 0.0f, 0.0f, -1.0f });
	// ������ת����
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(GetOrientation());

	XMFLOAT3 ForwardDirection;

	XMFLOAT3 ForwardVector = { 0.0f, 0.0f, 1.0f };

	XMStoreFloat3(&ForwardDirection, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), rotationMatrix));

	return ForwardDirection;
}

XMVECTOR CEditCamera::GetOrientation() const
{
	// ������Ԫ����ʾ��ת(����ŷ�������ݴ�����Ԫ����
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(-m_Pitch, -m_Yaw, 0.0f);

	return quaternion;

	// return glm::quat(glm::vec3{ -m_Pitch, -m_Yaw, 0.0f });
}

void CEditCamera::UpdateProjection()
{
	m_AspectRatio = FEngineRenderConfig::GetRenderConfig()->ScreenWidth / FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	
	XMStoreFloat4x4(&m_Projection, XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip));
}

void CEditCamera::UpdateView()
{
	// m_Yaw = m_Pitch = 0.0f;	// ���������ת
	m_Position = CalculatePosition();

	XMVECTOR orientation = GetOrientation();
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(orientation);

	XMVECTOR positionVector = XMLoadFloat3(&m_Position);
	XMMATRIX positionMatrix = XMMatrixTranslationFromVector(positionVector);

	XMMATRIX ret = XMMatrixMultiply(positionMatrix, translationMatrix);

	// ת��
	ret = XMMatrixTranspose(ret);

	XMStoreFloat4x4(&m_ViewMatrix, ret);

}

void CEditCamera::OnMouseScroll(int X, int Y, float InDelta)
{

	ENGINE_LOG("�����֣�%d, %d, %f", X, Y, InDelta);

	float delta = InDelta * 0.01f;
	MouseZoom(delta);
	UpdateView();
}

void CEditCamera::MousePan(const XMFLOAT2& delta)
{
	auto [xSpeed, ySpeed] = PanSpeed();
	m_FocalPoint.x += -GetRightDirection().x * delta.x * xSpeed * m_Distance;
	m_FocalPoint.y += -GetRightDirection().y * delta.x * xSpeed * m_Distance;
	m_FocalPoint.z += -GetRightDirection().z * delta.x * xSpeed * m_Distance;

	m_FocalPoint.x += GetUpDirection().x * delta.y * ySpeed * m_Distance;
	m_FocalPoint.y += GetUpDirection().y * delta.y * ySpeed * m_Distance;
	m_FocalPoint.z += GetUpDirection().z * delta.y * ySpeed * m_Distance;
}

void CEditCamera::MouseRotate(const XMFLOAT2& delta)
{
	float yawSign = GetUpDirection().y < 0 ? -1 : 1.0f;
	m_Yaw += yawSign * delta.x * RotationSpeed();
	m_Pitch += delta.y * RotationSpeed();
}

void CEditCamera::MouseZoom(float delta)
{
	m_Distance -= delta * ZoomSpeed();
	if (m_Distance < 1.0f)
	{
		m_FocalPoint.x += GetForwardDirection().x;
		m_FocalPoint.y += GetForwardDirection().y;
		m_FocalPoint.z += GetForwardDirection().z;
		m_Distance = 1.0f;
	}
}

XMFLOAT3 CEditCamera::CalculatePosition() const
{
	XMFLOAT3 ret = {
		m_FocalPoint.x - GetForwardDirection().x * m_Distance,
		m_FocalPoint.y - GetForwardDirection().y * m_Distance,
		m_FocalPoint.z - GetForwardDirection().z * m_Distance,
	};
	return ret;
}

std::pair<float, float> CEditCamera::PanSpeed() const
{
	float x = min(m_ViewportWidth / 1000.0f, 2.4f);	// max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = min(m_ViewportHeight / 1000.0f, 2.4f);	// max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float CEditCamera::RotationSpeed() const
{
	return 0.8f;
}

float CEditCamera::ZoomSpeed() const
{
	float distance = m_Distance * 0.2f;
	distance = max(distance, 0.0f);
	float speed = distance * distance;
	speed = min(speed, 100.0f); // max speed = 100
	return speed;
}
