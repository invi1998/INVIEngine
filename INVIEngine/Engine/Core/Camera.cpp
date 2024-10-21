#include "EngineMinimal.h"

#include "Camera.h"

#include "Component/InputComponent.h"
#include "Component/TransformationComponent.h"

CCamera::CCamera()
{
	FCreateObjectParams params{};
	params.Owner = this;
	params.ParentComponent = GetRootComponent();

	InputComponent = CreateObject<CInputComponent>(params, new CInputComponent());
}

void CCamera::BeginInit()
{
	// 初始化投影矩阵
	ViewPortInit();

	// 绑定键盘鼠标事件
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
		// W按下
		MoveForward(1.f);
	}
	else if (FInput::IsKeyPressed(Key::S))
	{
		// S按下
		MoveForward(-1.f);
	}
	else if (FInput::IsKeyPressed(Key::D))
	{
		// D按下
		MoveRight(1.f);
	}
	else if (FInput::IsKeyPressed(Key::A))
	{
		// A按下
		MoveRight(-1.f);
	}
	else if (FInput::IsKeyPressed(VK_LMENU))
	{
		// 左alt按下，进入鼠标移动摄像机视角事件
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

	// 传入弧度或得绕y轴的旋转矩阵
	XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

	// 将摄像机的3个方向向量乘以旋转矩阵，或得计算机绕z轴旋转的向量结果

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

	// 传入弧度或得绕Z轴的旋转矩阵
	XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

	// 将摄像机的3个方向向量乘以旋转矩阵，或得计算机绕z轴旋转的向量结果

	XMStoreFloat3(&GetRootComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));

	XMStoreFloat3(&GetRootComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationZ));

	XMStoreFloat3(&GetRootComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));

	BuildViewMatrix();

}
