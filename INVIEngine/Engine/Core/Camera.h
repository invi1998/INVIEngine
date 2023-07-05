#pragma once
#include "ViewPort.h"
#include "ColdeReflection/CodeReflectionMacro.h"
#include "CoreObject/CoreMinimalObject.h"

class CInputComponent;
class CTransformationComponent;

class CCamera : public CCoreMinimalObject, public FViewPort
{
public:
	~CCamera() override;

private:
	CVARIABLE();
	CInputComponent* InputComponent;

	CVARIABLE();
	CTransformationComponent* TransformationComponent;
};

