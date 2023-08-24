#pragma once

#include "EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"

class GQuaternionCamera;
class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	virtual ~CWorld() override;

	CCamera* GetCamera() const { return Camera; }

	GQuaternionCamera* GetQuaternionCamera() const { return QuaternionCamera; }

private:
	CVARIABLE()
		CCamera* Camera;

	GQuaternionCamera* QuaternionCamera;
};

