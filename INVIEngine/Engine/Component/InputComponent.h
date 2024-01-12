#pragma once
#include "Core/Component.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void);			// ������Ϣ��������
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureMouseInfoDelegate, void);			// �����Ϣ��������
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int, float);		// �������¼�
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);		// ����ƶ��¼�
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseRightDownDelegate, void, int, int);	// ����Ҽ������¼�
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseLeftDownDelegate, void, int, int);		// �����������¼�
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseRightUpDelegate, void, int, int);		// ����Ҽ�̧���¼�
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseLeftUpDelegate, void, int, int);		// ������̧���¼�

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

