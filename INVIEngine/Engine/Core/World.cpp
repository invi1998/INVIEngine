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

bool CWorld::LineTraceSingle(EngineType::FHitResult& hitResult, XMFLOAT3 start, XMFLOAT3 end)
{
	

	return false;
}
