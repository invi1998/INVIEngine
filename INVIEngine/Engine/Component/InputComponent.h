#pragma once
#include "Core/Component.h"
#include "EngineMinimal.h"
#include "Input/Input.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void);			// 键盘消息单播定义
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureMouseInfoDelegate, void);			// 鼠标消息单播定义

class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInfoDelegate CaptureKeyboardInfoDelegate;

	CVARIABLE()
	FCaptureMouseInfoDelegate CaptureMouseInfoDelegate;

public:
	~CInputComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;

};

