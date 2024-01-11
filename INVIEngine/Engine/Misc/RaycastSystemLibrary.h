#pragma once
#include "EngineType.h"


class CWorld;

struct FRayCastSystemLibrary
{
	static bool GetRaycastByscreen(CWorld* world, const XMFLOAT2& mousePos, XMVECTOR& originPoint, XMVECTOR& direction, XMMATRIX& viewInverseMatrix);

	static void GetHitResultByScreen(CWorld* world, const XMFLOAT2& mousePos, EngineType::FHitResult& OutHitResult);

	// 检测特定的物体是否被鼠标选中
	static bool CheckObjectIsSelected(CWorld* world, const XMFLOAT2& mousePos, const GActorObject* actor, EngineType::FHitResult& OutHitResult);
};

