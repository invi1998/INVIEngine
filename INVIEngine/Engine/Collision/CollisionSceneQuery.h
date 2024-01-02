#pragma once
#include "EngineType.h"

class CWorld;

// ��ײ��ѯ
struct FCollisionSceneQuery
{
	// ��ײ��ѯ
	static bool RayCastSingleQuery(CWorld* world, const XMFLOAT3& origin, const XMFLOAT3& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult);
};

