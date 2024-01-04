#pragma once

struct FRenderingData;
class GActorObject;
class CComponent;

namespace EngineType
{
	// ���н��
	struct FHitResult
	{
		bool bHit = false;					// �Ƿ�����
		XMFLOAT3 HitPoint{};				// ���е�
		XMFLOAT3 HitNormal{};				// ���е㷨��
		XMFLOAT3 HitDirection{};			// ���еķ���

		float HitDistance=0;			// ���о���
		float Time=0;				// ����ʱ��


		CComponent* HitComponent = nullptr;	// ���е����
		GActorObject* HitActor=nullptr;		// ���е�Actor

		std::weak_ptr<FRenderingData> HitRenderingData;	// ���е���Ⱦ����
		
	};
}


