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
	// 初始化投影矩阵
	ViewPortInit();

	// 绑定键盘事件
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &CCamera::ExecuteInput);

	// 绑定鼠标事件
	InputComponent->CaptureOnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
	InputComponent->CaptureOnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
	InputComponent->CaptureOnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
}

void CCamera::Tick(float DeltaTime)
{
	// BuildViewMatrix();
}

void CCamera::ExecuteInput(FInput& Input)
{
	if (Input.IsKeyPressed(Key::W))
	{
		// W按下
		MoveForward(1.f);
	}
	else if (Input.IsKeyPressed(Key::S))
	{
		// S按下
		MoveForward(-1.f);
	}
	else if (Input.IsKeyPressed(Key::D))
	{
		// D按下
		MoveRight(1.f);
	}
	else if (Input.IsKeyPressed(Key::A))
	{
		// A按下
		MoveRight(-1.f);
	}
	
}

void CCamera::BuildViewMatrix()
{
	// 构建viewMatrix
	ViewMatrix = TransformationComponent->CalculateViewMatrix();
}

void CCamera::OnMouseButtonDown(int x, int y)
{
	LastMousePosition.x = x;
	LastMousePosition.y = y;

	bIsLeftMouseDown = true;

	// 设置捕获
	SetCapture(GetMainWindowsHandle());
}

void CCamera::OnMouseButtonUp(int x, int y)
{
	bIsLeftMouseDown = false;
	// 释放捕获
	ReleaseCapture();
}

void CCamera::OnMouseMove(int x, int y)
{
	if (bIsLeftMouseDown)
	{
		float XRadians = XMConvertToRadians(static_cast<float>(x - LastMousePosition.x));
		float YRadians = XMConvertToRadians(static_cast<float>(y - LastMousePosition.y));
		RotateAroundYAxis(YRadians);
		RotateAroundZAxis(XRadians);
	}

	LastMousePosition.x = x;
	LastMousePosition.y = y;
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
}

void CCamera::RotateAroundZAxis(float InRotateDegrees)
{
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	// 传入弧度或得绕Z轴的旋转矩阵
	XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

	// 将摄像机的3个方向向量乘以旋转矩阵，或得计算机绕z轴旋转的向量结果
	XMFLOAT3 Right, Up, Forward;

	XMStoreFloat3(&Right, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
	TransformationComponent->SetRightVector(Right);

	XMStoreFloat3(&Up, XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationZ));
	TransformationComponent->SetRightVector(Up);

	XMStoreFloat3(&Forward, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));
	TransformationComponent->SetRightVector(Forward);

	BuildViewMatrix();

}
