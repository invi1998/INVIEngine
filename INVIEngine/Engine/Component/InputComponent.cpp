#include "EngineMinimal.h"

#include "InputComponent.h"

CInputComponent::~CInputComponent()
{
}

void CInputComponent::BeginInit()
{
	CComponent::BeginInit();

	MousesWheelsDelegate.AddFunction(this, &CInputComponent::OnMouseWheel);

	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMoved);

}

void CInputComponent::Tick(float DeltaTime)
{
	CComponent::Tick(DeltaTime);

	FInput Input;

	if (CaptureKeyboardInfoDelegate.IsBound())
	{
		CaptureKeyboardInfoDelegate.Execute();
	}

	if (CaptureMouseInfoDelegate.IsBound())
	{
		CaptureMouseInfoDelegate.Execute();
	}
}

void CInputComponent::OnMouseWheel(int X, int Y, float InDelta)
{
	if (OnMouseWheelDelegate.IsBound())
	{
		OnMouseWheelDelegate.Execute((int)X, (int)Y, (float)InDelta);
	}
}

void CInputComponent::OnMouseMoved(int x, int y)
{
	if (OnMouseMoveDelegate.IsBound())
	{
		OnMouseMoveDelegate.Execute((int)x, (int)y);
	}
}

