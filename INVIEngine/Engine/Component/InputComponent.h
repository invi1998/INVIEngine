#pragma once
#include "Core/Component.h"
#include "EngineMinimal.h"
#include "Input/Input.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void, const FInput&);	// 键盘消息单播定义
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureMouseInfoDelegate, void, int, int);			// 鼠标消息单播定义

class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInfoDelegate CaptureKeyboardInfoDelegate;

	CVARIABLE()
	FCaptureMouseInfoDelegate CaptureOnMouseButtonUpDelegate;

	CVARIABLE()
	FCaptureMouseInfoDelegate CaptureOnMouseButtonDownDelegate;

	CVARIABLE()
	FCaptureMouseInfoDelegate CaptureOnMouseMoveDelegate;

public:
	~CInputComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;

	virtual void OnMouseButtonUp(int x, int y);
	virtual void OnMouseButtonDown(int x, int y);
	virtual void OnMouseMove(int x, int y);

};

