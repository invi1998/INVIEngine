#pragma once
#include "EngineType.h"

class CWorld;

// ��ײ��ѯ
struct FCollisionSceneQuery
{
	static void GetRaycastParam(std::shared_ptr<FRenderingData>& renderData, const XMVECTOR& originPoint, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, XMVECTOR& outOriginPoint, XMVECTOR& outDirection);

	// ��ײ��ѯ
	static bool RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult);

	// ����ض��������Ƿ����ѡ��
	static bool RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult, const GActorObject* actor, std::vector<CComponent*>& ignoreComponents);

	static bool IsIgnoreComponent(CComponent* component, std::vector<CComponent*>& ignoreComponents);
};

