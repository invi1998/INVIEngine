#pragma once

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void);			// 键盘消息单播定义
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureMouseInfoDelegate, void);			// 鼠标消息单播定义
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int, float);		// 鼠标滚轮事件
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);		// 鼠标移动事件
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseRightDownDelegate, void, int, int);	// 鼠标右键按下事件
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseLeftDownDelegate, void, int, int);		// 鼠标左键按下事件
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseRightUpDelegate, void, int, int);		// 鼠标右键抬起事件
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseLeftUpDelegate, void, int, int);		// 鼠标左键抬起事件

class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInfoDelegate CaptureKeyboardInfoDelegate;

	CVARIABLE()
	FCaptureMouseInfoDelegate CaptureMouseInfoDelegate;

	CVARIABLE()
	FCaptureOnMouseWheelDelegate OnMouseWheelDelegate;

	CVARIABLE()
	FCaptureOnMouseDelegate OnMouseMoveDelegate;

	CVARIABLE()
	FCaptureOnMouseRightDownDelegate OnMouseRightDownDelegate;

	CVARIABLE()
	FCaptureOnMouseLeftDownDelegate OnMouseLeftDownDelegate;

	CVARIABLE()
	FCaptureOnMouseRightUpDelegate OnMouseRightUpDelegate;

	CVARIABLE()
	FCaptureOnMouseLeftUpDelegate OnMouseLeftUpDelegate;

public:
	~CInputComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;

	virtual void OnMouseWheel(int X, int Y, float InDelta);
	virtual void OnMouseMoved(int x, int y);

	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseRightUp(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);

};

