#include "InputComponent.h"

CInputComponent::~CInputComponent()
{
}

void CInputComponent::BeginInit()
{
	CComponent::BeginInit();
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

