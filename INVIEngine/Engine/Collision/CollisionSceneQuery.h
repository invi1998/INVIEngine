#pragma once
#include "EngineType.h"

class CWorld;

// ��ײ��ѯ
struct FCollisionSceneQuery
{
	// ��ײ��ѯ
	static bool RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult);
};

