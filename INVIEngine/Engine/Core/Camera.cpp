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

	// �󶨼����¼�
	InputComponent->CaptureKeyboardInfoDelegate.Bind(this, &CCamera::ExecuteInput);

	// ������¼�
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
		// W����
		MoveForward(1.f);
	}
	else if (Input.IsKeyPressed(Key::S))
	{
		// S����
		MoveForward(-1.f);
	}
	else if (Input.IsKeyPressed(Key::D))
	{
		// D����
		MoveRight(1.f);
	}
	else if (Input.IsKeyPressed(Key::A))
	{
		// A����
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
