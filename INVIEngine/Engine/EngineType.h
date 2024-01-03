#pragma once

class GActorObject;
class CComponent;

namespace EngineType
{
	// ���н��
	struct FHitResult
	{
		bool bHit = false;			// �Ƿ�����
		XMFLOAT3 Location{};		// ���е�
		XMFLOAT3 Normal{};			// ���е㷨��

		float Distance=0;			// ���о���
		float Time=0;				// ����ʱ��


		CComponent* HitComponent = nullptr;	// ���е����
		GActorObject* HitActor=nullptr;		// ���е�Actor
	};
}


