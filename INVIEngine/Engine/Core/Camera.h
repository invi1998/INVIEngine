#pragma once

#include "ColdeReflection/CodeReflectionMacro.h"
#include "CoreObject/CoreMinimalObject.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Viewport/ClientViewPort.h"
#include "Viewport/ViewPort.h"

class CInputComponent;
class CTransformationComponent;

class CCamera : public GClientViewPort, public IDirectXDeviceInterface
{
public:
	CCamera();

	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInput();

public:
	virtual void OnMouseMove(const XMFLOAT2& delta);

	virtual void MoveForward(float InValue);
	virtual void MoveRight(float InValue);

public:

	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }

protected:
	void RotateAroundYAxis(float InRotateDegrees);		// »∆Y÷·–˝◊™
	void RotateAroundZAxis(float InRotateDegrees);		// »∆Z÷·–˝◊™

	float MouseSensitivity = 0.003f;		//  Û±Í¡È√Ù∂»

private:
	CVARIABLE();
	CInputComponent* InputComponent;

	CVARIABLE();
	CTransformationComponent* TransformationComponent;

protected:
	XMFLOAT2 LastMousePosition = {0.f, 0.f};
	bool bIsLeftMouseDown = false;
};

