#pragma once
#include "Core/Component.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void);			// ������Ϣ��������
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureMouseInfoDelegate, void);			// �����Ϣ��������
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int, float);		// �������¼�

class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInfoDelegate CaptureKeyboardInfoDelegate;

	CVARIABLE()
	FCaptureMouseInfoDelegate CaptureMouseInfoDelegate;

	CVARIABLE()
	FCaptureOnMouseWheelDelegate OnMouseWheelDelegate;

public:
	~CInputComponent() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;

	virtual void OnMouseWheel(int X, int Y, float InDelta);

};

