#pragma once
#include "EngineType.h"


class CWorld;

struct FRayCastSystemLibrary
{
	static bool GetRayCastParamByScreen(CWorld* world, const XMFLOAT2& mousePos, XMVECTOR& originPoint, XMVECTOR& direction, XMMATRIX& viewInverseMatrix);	// ��ȡ��Ļ�����µ����߲���

	static void GetHitResultByScreen(CWorld* world, const XMFLOAT2& mousePos, EngineType::FHitResult& OutHitResult);

	// ����ض��������Ƿ����ѡ��
	static bool CheckObjectIsSelected(CWorld* world, const XMFLOAT2& mousePos, const GActorObject* actor, EngineType::FHitResult& OutHitResult, std::vector<CComponent*>& ignoreComponents);
};

