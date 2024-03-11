#include "EngineMinimal.h"

#include "Camera.h"

#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"

CCamera::CCamera()
{
	FCreateObjectParams params{};
	params.Owner = this;

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
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
	GClientViewPort::Tick(DeltaTime);
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
	XMFLOAT3 AT3Pos = GetRootComponent()->GetPosition();
	XMFLOAT3 AT3Forward = GetRootComponent()->GetForwardVector();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);

	XMVECTOR Forward = XMLoadFloat3(&AT3Forward);
	XMVECTOR Position = XMLoadFloat3(&AT3Pos);

	XMStoreFloat3(&AT3Pos, XMVectorMultiplyAdd(AmountMovement, Forward, Position));

	GetRootComponent()->SetPosition(AT3Pos);

	BuildViewMatrix();
}

void CCamera::MoveRight(float InValue)
{
	XMFLOAT3 AT3Pos = GetRootComponent()->GetPosition();
	XMFLOAT3 AT3Right = GetRootComponent()->GetRightVector();

	XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);

	XMVECTOR Right = XMLoadFloat3(&AT3Right);
	XMVECTOR Position = XMLoadFloat3(&AT3Pos);

	XMStoreFloat3(&AT3Pos, XMVectorMultiplyAdd(AmountMovement, Right, Position));

	GetRootComponent()->SetPosition(AT3Pos);

	BuildViewMatrix();
}

void CCamera::RotateAroundYAxis(float InRotateDegrees)
{
	XMFLOAT3 RightVector = GetRootComponent()->GetRightVector();
	XMFLOAT3 UpVector = GetRootComponent()->GetUpVector();
	XMFLOAT3 ForwardVector = GetRootComponent()->GetForwardVector();

	// ���뻡�Ȼ����y�����ת����
	XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

	// ���������3����������������ת���󣬻�ü������z����ת���������

	XMStoreFloat3(&GetRootComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));

	XMStoreFloat3(&GetRootComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));

	XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));

	BuildViewMatrix();

}

void CCamera::RotateAroundZAxis(float InRotateDegrees)
{
	XMFLOAT3 RightVector = GetRootComponent()->GetRightVector();
	XMFLOAT3 UpVector = GetRootComponent()->GetUpVector();
	XMFLOAT3 ForwardVector = GetRootComponent()->GetForwardVector();

	// ���뻡�Ȼ����Z�����ת����
	XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

	// ���������3����������������ת���󣬻�ü������z����ת���������

	XMStoreFloat3(&GetRootComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));

	XMStoreFloat3(&GetRootComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationZ));

	XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));

	BuildViewMatrix();

}
