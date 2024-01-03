#pragma once
#include "EngineType.h"

class CWorld;

// Åö×²²éÑ¯
struct FCollisionSceneQuery
{
	// Åö×²²éÑ¯
	static bool RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult);
};

