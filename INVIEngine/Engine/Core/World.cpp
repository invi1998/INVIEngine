#include "EngineMinimal.h"

#include "World.h"

#include "Camera.h"
#include "QuaternionCamera.h"

CWorld::CWorld()
{
	Camera = CreateObject<CCamera>(new CCamera());

	QuaternionCamera = CreateObject<GQuaternionCamera>(new GQuaternionCamera());
}

CWorld::~CWorld()
{
}
