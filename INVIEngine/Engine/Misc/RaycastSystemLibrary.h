#pragma once
#include "EngineType.h"


class CWorld;

struct FRayCastSystemLibrary
{
	static bool GetRayCastParamByScreen(CWorld* world, const XMFLOAT2& mousePos, XMVECTOR& originPoint, XMVECTOR& direction, XMMATRIX& viewInverseMatrix);	// 获取屏幕坐标下的射线参数

	static void GetHitResultByScreen(CWorld* world, const XMFLOAT2& mousePos, EngineType::FHitResult& OutHitResult);

	// 检测特定的物体是否被鼠标选中
	static bool CheckObjectIsSelected(CWorld* world, const XMFLOAT2& mousePos, const GActorObject* actor, EngineType::FHitResult& OutHitResult, std::vector<CComponent*>& ignoreComponents);
};

