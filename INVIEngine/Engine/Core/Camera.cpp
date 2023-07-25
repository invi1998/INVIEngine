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

void CCamera::OnMouseButtonDown(int x, int y)
{
	// LastMousePosition.x = x;
	// LastMousePosition.y = y;
}

void CCamera::OnMouseButtonUp(int x, int y)
{
}

void CCamera::OnMouseMove(int x, int y)
{
}

void CCamera::MoveForward(float InValue)
{

}

void CCamera::MoveRight(float InValue)
{
}
