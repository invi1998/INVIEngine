#include "World.h"

#include "Camera.h"
#include "QuaternionCamera.h"

CWorld::CWorld()
{
	Camera = CreateObject<CCamera>(new CCamera());

	QuaternionCamera = CreateObject<CQuaternionCamera>(new CQuaternionCamera());
}

CWorld::~CWorld()
{
}
