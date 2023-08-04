#include "World.h"

#include "Camera.h"
#include "EditCamera.h"

CWorld::CWorld()
{
	Camera = CreateObject<CCamera>(new CCamera());

	EditCamera = CreateObject<CEditCamera>(new CEditCamera());
}

CWorld::~CWorld()
{
}
