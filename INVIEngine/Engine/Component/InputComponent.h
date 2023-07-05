#pragma once
#include "Core/Component.h"
#include "EngineMinimal.h"
#include "Input/Input.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void, const FInput&)

class CInputComponent : public CComponent
{
public:
	~CInputComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;

	CVARIABLE();
	FCaptureKeyboardInfoDelegate CaptureKeyboardInfoDelegate;
};

