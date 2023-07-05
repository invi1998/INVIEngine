#pragma once

#include "EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"

class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	virtual ~CWorld() override;

private:
	CVARIABLE()
		CCamera* Camera;


};

