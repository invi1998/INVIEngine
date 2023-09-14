#pragma once

#include "EngineMinimal.h"
#include "CoreObject/CoreMinimalObject.h"

class GActorObject;
class GQuaternionCamera;
class CCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	virtual ~CWorld() override;

	CCamera* GetCamera() const { return Camera; }

	GQuaternionCamera* GetQuaternionCamera() const { return QuaternionCamera; }

	template<typename T>
	T* CreateActorObject()
	{
		T* InArray = new T();
		ActorObjects.push_back(InArray);

		return InArray;
	}

private:
	CVARIABLE()
		CCamera* Camera;

	CVARIABLE()
	GQuaternionCamera* QuaternionCamera;

	CVARIABLE()
		vector<GActorObject*> ActorObjects;
};

