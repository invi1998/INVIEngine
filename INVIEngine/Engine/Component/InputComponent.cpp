#include "InputComponent.h"

CInputComponent::~CInputComponent()
{
}

void CInputComponent::BeginInit()
{
	CComponent::BeginInit();

	// ��ʼ���׶ΰ󶨴����ִ�к���
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);

}

void CInputComponent::Tick(float DeltaTime)
{
	CComponent::Tick(DeltaTime);

	if (CaptureKeyboardInfoDelegate.IsBound())
	{
		FInput Input;
		CaptureKeyboardInfoDelegate.Execute(Input);
	}
}

void CInputComponent::OnMouseButtonUp(int x, int y)
{
	if (CaptureOnMouseButtonUpDelegate.IsBound())
	{
		// ����Ѿ��ɹ����˴���
		CaptureOnMouseButtonUpDelegate.Execute(x, y);
	}
}

void CInputComponent::OnMouseButtonDown(int x, int y)
{
	if (CaptureOnMouseButtonDownDelegate.IsBound())
	{
		CaptureOnMouseButtonDownDelegate.Execute(x, y);
	}
}

void CInputComponent::OnMouseMove(int x, int y)
{
	if (CaptureOnMouseMoveDelegate.IsBound())
	{
		CaptureOnMouseMoveDelegate.Execute(x, y);
	}
}
