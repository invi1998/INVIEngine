#pragma once

#include "EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"

class CEditCamera;
class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	virtual ~CWorld() override;

	CCamera* GetCamera() const { return Camera; }

	CEditCamera* GetEditCamera() const { return EditCamera; }

private:
	CVARIABLE()
		CCamera* Camera;

	CEditCamera* EditCamera;
};

