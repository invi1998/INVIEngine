#pragma once

#include "CoreObject/CoreMinimalObject.h"

#include "EngineType.h"

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

	// µ•…‰œﬂºÏ≤‚
	bool LineTraceSingle(EngineType::FHitResult& hitResult, XMFLOAT3 start, XMFLOAT3 end);

private:
	CVARIABLE()
		CCamera* Camera;

	CVARIABLE()
	GQuaternionCamera* QuaternionCamera;

	CVARIABLE()
		vector<GActorObject*> ActorObjects;
};

