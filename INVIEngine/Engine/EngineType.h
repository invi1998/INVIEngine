#pragma once

class GActorObject;
class CComponent;

namespace EngineType
{
	// 命中结果
	struct FHitResult
	{
		bool bHit = false;			// 是否命中
		XMFLOAT3 Location{};		// 命中点
		XMFLOAT3 Normal{};			// 命中点法线

		float Distance=0;			// 命中距离
		float Time=0;				// 命中时间


		CComponent* HitComponent = nullptr;	// 命中的组件
		GActorObject* HitActor=nullptr;		// 命中的Actor
	};
}


