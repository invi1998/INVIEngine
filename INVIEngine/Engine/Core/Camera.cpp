#include "Camera.h"

#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

CCamera::~CCamera()
{
}

void CCamera::BeginInit()
{
	// ��ʼ��ͶӰ����
	ViewPortInit();

	// �󶨼�������¼�
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &CCamera::ExecuteInput);
}

void CCamera::Tick(float DeltaTime)
{
	// BuildViewMatrix();
}

void CCamera::ExecuteInput()
{
	if (FInput::IsKeyPressed(Key::W))
	{
		// W����
		MoveForward(1.f);
	}
	else if (FInput::IsKeyPressed(Key::S))
	{
		// S����
		MoveForward(-1.f);
	}
	else if (FInput::IsKeyPressed(Key::D))
	{
		// D����
		MoveRight(1.f);
	}
	else if (FInput::IsKeyPressed(Key::A))
	{
		// A����
		MoveRight(-1.f);
	}
	else if (FInput::IsKeyPressed(VK_LMENU))
	{
		// ��alt���£���������ƶ�������ӽ��¼�
		const XMFLOAT2 &mouse{ FInput::GetMouseX(), FInput::GetMouseY() };

		XMVECTOR VMouse = XMLoadFloat2(&mouse);
		XMVECTOR VLastMousePosition = XMLoadFloat2(&LastMousePosition);

		XMVECTOR VDelta = XMVectorSubtract(VMouse, VLastMousePosition) * 0.003f;

		XMFLOAT2 delta;
		XMStoreFloat2(&delta, VDelta);

		LastMousePosition = mouse;

		if (FInput::IsMouseButtonPressed(VK_LBUTTON))
		{
			OnMouseMove(delta);
		}
		
	}
}

void CCamera::BuildViewMatrix()
{
	// ����viewMatrix
	ViewMatrix = TransformationComponent->CalculateViewMatrix();
}

void CCamera::OnMouseMove(const XMFLOAT2& delta)
{
	float XRadians = XMConvertToRadians(delta.x - LastMousePosition.x) * MouseSensitivity;
	float YRadians = XMConvertToRadians(delta.y - LastMousePosition.y) * MouseSensitivity;

	ENGINE_LOG("XRadians %f", XRadians);
	ENGINE_LOG("YRadians %f", YRadians);

	RotateAroundYAxis(YRadians);
	RotateAroundZAxis(XRadians);
}

void CCamera::MoveForward(float InValue)
{
	XMFLOAT3 AT3Pos = TransformationComponent->GetPosition();
	XMFLOAT3 AT3Forward = TransformationComponent->GetForwardVector();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);

	XMVECTOR Forward = XMLoadFloat3(&AT3Forward);
	XMVECTOR Position = XMLoadFloat3(&AT3Pos);

	XMStoreFloat3(&AT3Pos, XMVectorMultiplyAdd(AmountMovement, Forward, Position));

	TransformationComponent->SetPosition(AT3Pos);

	BuildViewMatrix();
}

void CCamera::MoveRight(float InValue)
{
	XMFLOAT3 AT3Pos = TransformationComponent->GetPosition();
	XMFLOAT3 AT3Right = TransformationComponent->GetRightVector();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);

	XMVECTOR Right = XMLoadFloat3(&AT3Right);
	XMVECTOR Position = XMLoadFloat3(&AT3Pos);

	XMStoreFloat3(&AT3Pos, XMVectorMultiplyAdd(AmountMovement, Right, Position));

	TransformationComponent->SetPosition(AT3Pos);

	BuildViewMatrix();
}

void CCamera::RotateAroundYAxis(float InRotateDegrees)
{
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	// ���뻡�Ȼ����y�����ת����
	XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

	// ���������3����������������ת���󣬻�ü������z����ת���������

	XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));

	XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));

	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));

	BuildViewMatrix();

}

void CCamera::RotateAroundZAxis(float InRotateDegrees)
{
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	// ���뻡�Ȼ����Z�����ת����
	XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

	// ���������3����������������ת���󣬻�ü������z����ת���������

	XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));

	XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationZ));

	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));

	BuildViewMatrix();

}
