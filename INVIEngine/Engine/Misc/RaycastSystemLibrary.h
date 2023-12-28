#pragma once
#include "EngineType.h"


class CWorld;

struct FRayCastSystemLibrary
{
	static bool GetHitResultByScreen(CWorld* world, const XMFLOAT2& mousePos, EngineType::FHitResult& OutHitResult);
};

