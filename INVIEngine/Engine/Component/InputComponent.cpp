#include "InputComponent.h"

CInputComponent::~CInputComponent()
{
}

void CInputComponent::BeginInit()
{
	CComponent::BeginInit();

	// 初始化阶段绑定代理的执行函数
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
		// 如果已经成功绑定了代理
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
