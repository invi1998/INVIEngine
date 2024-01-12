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

	RightMouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseRightDown);

	LeftMouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseLeftDown);

	RightMouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseRightUp);

	LeftMouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseLeftUp);

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

void CInputComponent::OnMouseRightDown(int x, int y)
{
	if (OnMouseRightDownDelegate.IsBound())
	{
		OnMouseRightDownDelegate.Execute((int)x, (int)y);
	}
}

void CInputComponent::OnMouseLeftDown(int x, int y)
{
	if (OnMouseLeftDownDelegate.IsBound())
	{
		OnMouseLeftDownDelegate.Execute((int)x, (int)y);
	}
}

void CInputComponent::OnMouseRightUp(int x, int y)
{
	if (OnMouseRightUpDelegate.IsBound())
	{
		OnMouseRightUpDelegate.Execute((int)x, (int)y);
	}
}

void CInputComponent::OnMouseLeftUp(int x, int y)
{
	if (OnMouseLeftUpDelegate.IsBound())
	{
		OnMouseLeftUpDelegate.Execute((int)x, (int)y);
	}
}

