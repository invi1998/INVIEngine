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

	if (CaptureKeyboardInfoDelegate.IsBound())
	{
		FInput Input;
		CaptureKeyboardInfoDelegate.Execute(Input);
	}
}
