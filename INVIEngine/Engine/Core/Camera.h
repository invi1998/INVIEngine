#pragma once

#include "EngineMinimal.h"

#include "ColdeReflection/CodeReflectionMacro.h"
#include "CoreObject/CoreMinimalObject.h"
#include "Component/Input/Input.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Viewport/ViewPort.h"

class CInputComponent;
class CTransformationComponent;

class CCamera : public CCoreMinimalObject, public FViewPort, public IDirectXDeviceInterface
{
public:
	CCamera();
	~CCamera() override;

	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInput();

	void BuildViewMatrix();

public:
	virtual void OnMouseButtonDown(int x, int y);
	virtual void OnMouseButtonUp(int x, int y);
	virtual void OnMouseMove(int x, int y);

	virtual void MoveForward(float InValue);
	virtual void MoveRight(float InValue);

public:

	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }

protected:
	void RotateAroundYAxis(float InRotateDegrees);		// ÈÆYÖáÐý×ª
	void RotateAroundZAxis(float InRotateDegrees);		// ÈÆZÖáÐý×ª


private:
	CVARIABLE();
	CInputComponent* InputComponent;

	CVARIABLE();
	CTransformationComponent* TransformationComponent;

protected:
	POINT LastMousePosition;
	bool bIsLeftMouseDown = false;
};

