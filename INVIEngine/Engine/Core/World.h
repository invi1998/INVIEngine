#pragma once

#include "EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"

class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	virtual ~CWorld() override;

	CCamera* GetCamera() const { return Camera; }

private:
	CVARIABLE()
		CCamera* Camera;


};

