#pragma once
#include "ViewPort.h"
#include "ColdeReflection/CodeReflectionMacro.h"
#include "CoreObject/CoreMinimalObject.h"
#include "Component/Input/Input.h"

class CInputComponent;
class CTransformationComponent;

class CCamera : public CCoreMinimalObject, public FViewPort
{
public:
	CCamera();
	~CCamera() override;

	virtual void BeginInit() override;
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInput(const FInput& Input);

public:

	FORCEINLINE CInputComponent* GetInputComponent() const { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }


private:
	CVARIABLE();
	CInputComponent* InputComponent;

	CVARIABLE();
	CTransformationComponent* TransformationComponent;
};

