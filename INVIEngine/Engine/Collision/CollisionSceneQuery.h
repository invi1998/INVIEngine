#pragma once
#include "EngineType.h"

class CWorld;

// 碰撞查询
struct FCollisionSceneQuery
{
	static void GetRaycastParam(std::shared_ptr<FRenderingData>& renderData, const XMVECTOR& originPoint, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, XMVECTOR& outOriginPoint, XMVECTOR& outDirection);

	// 碰撞查询
	static bool RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult);

	// 检测特定的物体是否被鼠标选中
	static bool RayCastSingleQuery(CWorld* world, const XMVECTOR& origin, const XMVECTOR& direction, const XMMATRIX& viewInverseMatrix, EngineType::FHitResult& OutHitResult, const GActorObject* actor, std::vector<CComponent*>& ignoreComponents);

	static bool IsIgnoreComponent(CComponent* component, std::vector<CComponent*>& ignoreComponents);
};

