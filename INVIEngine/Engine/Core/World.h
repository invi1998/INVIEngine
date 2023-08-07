#pragma once

#include "EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"

class CQuaternionCamera;
class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	virtual ~CWorld() override;

	CCamera* GetCamera() const { return Camera; }

	CQuaternionCamera* GetQuaternionCamera() const { return QuaternionCamera; }

private:
	CVARIABLE()
		CCamera* Camera;

	CQuaternionCamera* QuaternionCamera;
};

