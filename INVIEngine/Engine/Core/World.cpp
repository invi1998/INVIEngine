#include "EngineMinimal.h"

#include "World.h"

#include "Camera.h"
#include "QuaternionCamera.h"

CWorld::CWorld()
{
	FCreateObjectParams params{};
	params.Owner = this;

	Camera = CreateObject<CCamera>(params, new CCamera());

	QuaternionCamera = CreateObject<GQuaternionCamera>(params, new GQuaternionCamera());
}

CWorld::~CWorld()
{
}

bool CWorld::LineTraceSingle(EngineType::FHitResult& hitResult, XMFLOAT3 start, XMFLOAT3 end)
{
	

	return false;
}
