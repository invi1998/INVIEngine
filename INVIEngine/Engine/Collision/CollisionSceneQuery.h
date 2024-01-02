#pragma once
#include "EngineType.h"

class CWorld;

// Åö×²²éÑ¯
struct FCollisionSceneQuery
{
	// Åö×²²éÑ¯
	static bool RayCastSingleQuery(CWorld* world, const XMFLOAT3& origin, const XMFLOAT3& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult);
};

